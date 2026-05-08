#ifndef CSVLOGGER_H
#define CSVLOGGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include "data/point_value.h"

// 前向声明
class QFile;
class QTextStream;
class ConfigManager;

// CSV 数据记录器
//   1. 每分钟一个文件，命名 data_yyyyMMdd_HHmm.csv
//   2. UTF-8 + BOM 编码，Excel 可以直接打开且中文不乱码
//   3. 自动处理列顺序变化（用户改配置后自动切新文件）
//   4. 程序重启后同一分钟继续追加，不覆盖原文件
class CsvLogger : public QObject {
    Q_OBJECT
public:
    explicit CsvLogger(QObject* parent = nullptr);
    ~CsvLogger();

    //  公共接口
    // 设置保存目录。空字符串 = 不写盘
    // 切换目录会自动关闭当前文件
    void setSaveDir(const QString& dir);
    // 获取当前保存目录
    QString saveDir() const { return _saveDir; }

    // 设置配置管理器（用来取点位列表，决定 CSV 列顺序）
    // 注意：_cfg 必须比 CsvLogger 活得久（实践中：MainWindow 持有两者）
    void setConfigManager(ConfigManager* cfg) { _cfg = cfg; }

public slots:
    // 接收 DataParser 输出，每秒写入一行数据
    void onPointsUpdated(QList<PointValue> points);

signals:
    // 写盘出错时发出（让外部状态栏提示用户）
    void errorOccurred(const QString& err);

private:
    //  核心成员
    QString _saveDir;              // 保存目录，空字符串 = 不写盘
    ConfigManager* _cfg = nullptr; // 配置管理器，取列顺序用

    //  文件状态
    QFile* _file = nullptr;        // 当前打开的文件
    QTextStream* _stream = nullptr; // 文件的文本流
    QString _currentMinuteKey;      // 当前文件对应的"yyyyMMdd_HHmm"
    QStringList _currentHeader;     // 当前文件的列顺序（用来检测配置变化）

    //  内部辅助函数
    // 把毫秒时间戳转成 "yyyyMMdd_HHmm"，作为文件名和切换判断的 key
    QString minuteKey(qint64 ms) const;

    // 从 _cfg 取当前应该写的列顺序（点位名列表）
    QStringList currentColumnOrder() const;

    // 关闭当前文件（先关流再关文件）
    void closeFile();

    // 打开新文件并写入表头（仅当文件为空时）
    void openFileFor(const QString& key, const QStringList& header);

    // 写入一行数据：时间 + 各个点位值
    void writeRow(qint64 ts, const QList<PointValue>& points);
};

#endif // CSVLOGGER_H