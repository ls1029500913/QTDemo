#include "dataparser.h"
#include "config/configmanager.h"
#include "config/point_config.h"

#include <QDateTime>

// 构造函数：绑定配置管理器
DataParser::DataParser(ConfigManager* cfg, QObject* parent)
    : QObject(parent), _cfg(cfg)
{
}

// 核心解析逻辑：原始寄存器值 → 工程值
void DataParser::onRawDataReceived(RegValueMap regValues)
{
    if (!_cfg) return;

    // 生成统一时间戳（本轮所有数据共用一个时间）
    const qint64 ts = QDateTime::currentMSecsSinceEpoch();
    QList<PointValue> result;

    // 遍历所有配置点位，转换工程值
    for (const PointConfig& p : _cfg->points()) {
        auto it = regValues.constFind(p.reg);
        if (it == regValues.cend())
            continue; // 没读到该寄存器，跳过

        // 原始值转工程值：工程值 = 原始值 × 倍率
        qint16 raw = static_cast<qint16>(it.value());
        result.append({ p.name, raw * p.scale, ts });
    }

    // 有数据则发送信号
    if (!result.isEmpty())
        emit pointsUpdated(result);
}