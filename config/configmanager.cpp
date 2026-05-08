#include "configmanager.h"

#include <QFile>
#include <QSet>
#include <QTextStream>
#include <QStringList>
#include <algorithm>

// 校验配置：名称非空、倍率非0、名称/寄存器不重复
QString ConfigManager::validate() const
{
    QSet<QString> names; // 已出现的点位名称
    QSet<quint16> regs;  // 已出现的寄存器地址

    for (int i = 0; i < _points.size(); ++i) {
        const PointConfig& p = _points.at(i);

        if (p.name.trimmed().isEmpty())
            return QString::fromUtf8("第 %1 行：点位名称不能为空").arg(i + 1);

        if (p.scale == 0.0)
            return QString::fromUtf8("第 %1 行：倍率不能为 0").arg(i + 1);

        if (names.contains(p.name))
            return QString::fromUtf8("点位名称重复：%1").arg(p.name);

        if (regs.contains(p.reg))
            return QString::fromUtf8("寄存器地址重复：%1").arg(p.reg);

        names.insert(p.name);
        regs.insert(p.reg);
    }
    return QString(); // 空串表示校验通过
}

// 聚合寄存器地址：把零散地址按间隔和最大长度合并成读取段
// mergeGap：允许的最大地址间隔（默认1）
QList<QPair<quint16, quint16>> ConfigManager::aggregateReadGroups(int mergeGap) const
{
    QList<QPair<quint16, quint16>> groups;
    if (_points.isEmpty())
        return groups;

    // 1. 提取所有寄存器地址并排序
    QList<quint16> regs;
    for (const auto& p : _points)
        regs.append(p.reg);
    std::sort(regs.begin(), regs.end());

    // Modbus TCP 单次最大读取长度（留余量）
    constexpr int kMaxPerGroup = 120;

    quint16 start = regs.first(); // 当前段起始地址
    quint16 last = start;          // 当前段最后地址

    for (int i = 1; i < regs.size(); ++i) {
        quint16 cur = regs.at(i);
        int gap = cur - last;
        int newCount = cur - start + 1;

        // 间隔在允许范围内且不超过最大长度 → 合并
        if (gap <= mergeGap && newCount <= kMaxPerGroup) {
            last = cur;
        } else {
            // 结束当前段，开启新段
            groups.append({start, static_cast<quint16>(last - start + 1)});
            start = cur;
            last = cur;
        }
    }
    // 追加最后一段
    groups.append({start, static_cast<quint16>(last - start + 1)});
    return groups;
}

// ------------------------------ CSV 辅助函数 ------------------------------
// CSV 字段转义：含逗号/引号/换行时加引号包裹，内部引号双写
static QString csvEscape(const QString& s)
{
    if (s.contains(',') || s.contains('"') || s.contains('\n')) {
        QString e = s;
        e.replace('"', "\"\"");
        return '"' + e + '"';
    }
    return s;
}

// 解析一行 CSV，处理带引号的字段
static QStringList csvSplitLine(const QString& line)
{
    QStringList result;
    QString current;
    bool inQuote = false;
    for (int i = 0; i < line.size(); ++i) {
        QChar c = line.at(i);
        if (inQuote) {
            if (c == '"') {
                if (i + 1 < line.size() && line.at(i + 1) == '"') {
                    current += '"';
                    ++i;
                } else {
                    inQuote = false;
                }
            } else {
                current += c;
            }
        } else {
            if (c == '"')
                inQuote = true;
            else if (c == ',') {
                result.append(current);
                current.clear();
            } else
                current += c;
        }
    }
    result.append(current);
    return result;
}

// 从 CSV 文件加载配置
QString ConfigManager::loadFromCsv(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString::fromUtf8("无法打开文件：%1").arg(path);

    QTextStream in(&f);
    in.setCodec("UTF-8");

    // 处理 BOM 头
    QString first = in.readLine();
    if (!first.isEmpty() && first.at(0).unicode() == 0xFEFF)
        first = first.mid(1);

    if (first.isEmpty())
        return QString::fromUtf8("文件为空");

    // 校验表头
    QStringList header = csvSplitLine(first);
    if (header.size() < 4)
        return QString::fromUtf8("表头列数不正确，应为 4 列");

    // 解析数据行
    QList<PointConfig> list;
    int lineNo = 1;
    while (!in.atEnd()) {
        ++lineNo;
        QString line = in.readLine();
        if (line.trimmed().isEmpty())
            continue;

        QStringList cols = csvSplitLine(line);
        if (cols.size() < 4)
            return QString::fromUtf8("第 %1 行：列数不足").arg(lineNo);

        PointConfig p;
        p.name = cols.at(0).trimmed();

        // 解析寄存器地址
        bool ok = false;
        int reg = cols.at(1).trimmed().toInt(&ok);
        if (!ok)
            return QString::fromUtf8("第 %1 行：寄存器无法解析").arg(lineNo);
        p.reg = static_cast<quint16>(reg);

        // 解析倍率
        p.scale = cols.at(2).trimmed().toDouble(&ok);
        if (!ok)
            return QString::fromUtf8("第 %1 行：倍率无法解析").arg(lineNo);

        // 解析是否可控制
        QString cstr = cols.at(3).trimmed().toLower();
        p.controllable = (cstr == "1" || cstr == "true"
                          || cstr == "yes" || cstr == QString::fromUtf8("是"));
        list.append(p);
    }

    // 校验并保存
    _points = list;
    QString verr = validate();
    if (!verr.isEmpty()) {
        _points.clear(); // 校验失败回滚
        return verr;
    }
    return QString();
}

// 保存配置到 CSV 文件
QString ConfigManager::saveToCsv(const QString& path)
{
    // 先校验
    QString verr = validate();
    if (!verr.isEmpty())
        return verr;

    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return QString::fromUtf8("无法写入文件：%1").arg(path);

    QTextStream out(&f);
    out.setCodec("UTF-8");
    out.setGenerateByteOrderMark(true); // 加 BOM 保证 Excel 中文不乱码

    // 写入表头
    out << QString::fromUtf8("点位名称,寄存器,倍率,是否可控制\n");
    // 写入数据行
    for (const auto& p : _points) {
        out << csvEscape(p.name) << ','
            << p.reg << ','
            << QString::number(p.scale, 'g', 10) << ','
            << (p.controllable ? '1' : '0') << '\n';
    }
    out.flush();
    return QString();
}