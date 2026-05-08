#ifndef POINT_CONFIG_H
#define POINT_CONFIG_H

#include <QString>

// CSV 配置
struct PointConfig {
    QString name; // 点位名称（唯一）
    quint16 reg = 0;   // Modbus 寄存器地址（唯一）
    double  scale = 1.0;   // 倍率：工程值 = 寄存器原始值 × scale
    bool    controllable = false;  // 是否可下发
};

#endif // POINT_CONFIG_H