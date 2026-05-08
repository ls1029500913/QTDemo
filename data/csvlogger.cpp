#include "csvlogger.h"
#include "config/configmanager.h"
#include "config/point_config.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QHash>

// 构造函数
CsvLogger::CsvLogger(QObject* parent) : QObject(parent) {}

// 析构函数：关闭文件，确保数据落盘
CsvLogger::~CsvLogger()
{
    closeFile();
}

// 设置保存目录：切换目录时自动关闭旧文件
void CsvLogger::setSaveDir(const QString& dir)
{
    closeFile();
    _saveDir = dir;

    // 确保目录存在（支持多级目录）
    if (!_saveDir.isEmpty())
        QDir().mkpath(_saveDir);
}

// 主入口：每秒接收一次数据，写入 CSV
void CsvLogger::onPointsUpdated(QList<PointValue> points)
{
    if (_saveDir.isEmpty()) return;
    if (!_cfg) return;

    // 取本轮第一个点的时间戳
    const qint64 ts = points.first().timestampMs;
    // 生成分钟级 key（同一分钟写同一个文件）
    const QString key = minuteKey(ts);
    // 获取当前列顺序（按配置顺序）
    const QStringList header = currentColumnOrder();

    // 判断是否需要切文件/开新文件：
    // 1. 当前没开文件
    // 2. 分钟变了
    // 3. 列顺序变了（用户改了配置）
    bool needNewFile = !_file
                       || key != _currentMinuteKey
                       || header != _currentHeader;

    if (needNewFile)
        openFileFor(key, header);

    // 打开失败则跳过本轮
    if (!_stream) return;

    writeRow(ts, points);
}

// -内部辅助函数
// 根据时间戳生成分钟级 key（yyyyMMdd_HHmm）
QString CsvLogger::minuteKey(qint64 ms) const
{
    return QDateTime::fromMSecsSinceEpoch(ms).toString("yyyyMMdd_HHmm");
}

// 获取当前配置的点位名称列表（决定 CSV 列顺序）
QStringList CsvLogger::currentColumnOrder() const
{
    QStringList names;
    if (!_cfg) return names;

    // 按配置顺序获取所有点位名称
    for (const PointConfig& p : _cfg->points())
        names << p.name;
    return names;
}

// 关闭当前文件：先关流再关文件，避免段错误
void CsvLogger::closeFile()
{
    // 先关闭流
    if (_stream) {
        _stream->flush();
        delete _stream;
        _stream = nullptr;
    }

    // 再关闭文件
    if (_file) {
        _file->close();
        delete _file;
        _file = nullptr;
    }

    // 重置状态
    _currentMinuteKey.clear();
    _currentHeader.clear();
}

// 打开新的 CSV 文件，自动写入表头
void CsvLogger::openFileFor(const QString& key, const QStringList& header)
{
    closeFile();

    // 文件路径：保存目录/data_20260504_1630.csv
    const QString fileName = QString("data_%1.csv").arg(key);
    const QString fullPath = QDir(_saveDir).filePath(fileName);

    _file = new QFile(fullPath);

    // 打开模式：追加 + 文本模式（自动处理换行符）
    // 程序重启后同一分钟继续追加，不覆盖
    if (!_file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        emit errorOccurred(QString::fromUtf8("CSV 打开失败：") + fullPath);
        return;
    }

    // 创建文本流，设置 UTF-8 编码
    _stream = new QTextStream(_file);
    _stream->setCodec("UTF-8");

    // 如果是新创建的空文件，写入 BOM + 表头
    if (_file->size() == 0) {
        _stream->setGenerateByteOrderMark(true); // BOM：Excel 打开中文不乱码

        // 表头：时间 + 各点位名
        *_stream << QString::fromUtf8("时间");
        for (const QString& name : header)
            *_stream << ',' << name;
        *_stream << '\n';
    }

    _currentMinuteKey = key;
    _currentHeader = header;
}

// 写入一行数据：时间 + 各个点位值
void CsvLogger::writeRow(qint64 ts, const QList<PointValue>& points)
{
    // 把本轮数据按名称建索引，方便按列顺序查找
    QHash<QString, double> byName;
    for (const PointValue& pv : points)
        byName.insert(pv.name, pv.value);

    // 第 1 列：时间（精确到毫秒）
    *_stream << QDateTime::fromMSecsSinceEpoch(ts)
                    .toString("yyyy-MM-dd HH:mm:ss.zzz");

    // 后续列：按表头顺序写入
    for (const QString& name : _currentHeader) {
        *_stream << ',';
        auto it = byName.constFind(name);
        if (it != byName.constEnd()) {
            // 自动格式，最多10位有效数字，不显示多余0
            *_stream << QString::number(it.value(), 'g', 10);
        }
        // 没值就写空字段（,,）
    }

    // 换行
    *_stream << '\n';
}