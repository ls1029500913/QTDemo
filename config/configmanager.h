#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QList>
#include <QPair>
#include "config/point_config.h"

// 配置管理器：负责点位配置的加载、保存、校验和聚合
class ConfigManager {
public:
    //  文件 IO
    // 从 CSV 文件加载配置（返回空字符串表示成功，否则为错误信息）
    QString loadFromCsv(const QString& path);
    // 保存配置到 CSV 文件（返回空字符串表示成功，否则为错误信息）
    QString saveToCsv(const QString& path);

    //  校验
    // 校验当前配置（返回空字符串表示通过，否则为错误信息）
    QString validate() const;

    //  点位列表操作
    // 设置点位列表
    void setPoints(const QList<PointConfig>& list) { _points = list; }
    // 获取点位列表
    const QList<PointConfig>& points() const { return _points; }

    //  寄存器聚合
    // 把零散寄存器地址合并成 (起始地址, 数量) 段，用于 Modbus 批量读取
    // mergeGap：允许的最大地址间隔（默认8）
    QList<QPair<quint16, quint16>> aggregateReadGroups(int mergeGap = 8) const;

private:
    QList<PointConfig> _points; // 用户配置的点位列表
};

#endif // CONFIGMANAGER_H