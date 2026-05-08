#ifndef POINT_VALUE_H
#define POINT_VALUE_H

#include <QString>

// 解析后的工程值
// 由 DataParser 输出，CurveView 和未来的 CsvLogger 消费
struct PointValue {
    QString name;          // 点位名（从配置里来，唯一）
    double  value = 0.0;   // 工程值（寄存器原始值 × scale）
    qint64  timestampMs = 0;  // 时间戳，毫秒
};

#endif // POINT_VALUE_H