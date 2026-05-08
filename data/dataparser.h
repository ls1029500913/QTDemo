#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>
#include <QList>
#include "data/point_value.h"
#include "communication/modbuscomm.h"

// 前向声明：只用指针，不需要完整类型
class ConfigManager;

// 数据解析层
// 把原始寄存器值转成"带名字、带工程值"的 PointValue 列表
// 上：ModbusComm.rawDataReceived(QHash<reg, raw_value>)
// 下：CurveView
class DataParser : public QObject {
    Q_OBJECT
public:
    // cfg 必须比 DataParser 时间久，防止悬空指针
    DataParser(ConfigManager* cfg, QObject* parent = nullptr);

public slots:
    // 接收 ModbusComm 的原始数据
    void onRawDataReceived(RegValueMap regValues);

signals:
    // 发送解析后的点位数据
    void pointsUpdated(QList<PointValue> points);

private:
    ConfigManager* _cfg;
};

#endif // DATAPARSER_H