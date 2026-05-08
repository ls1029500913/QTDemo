#ifndef POINT_VALUE_H
#define POINT_VALUE_H

#include <QString>

struct PointValue {
    QString name;
    double  value = 0.0;
    qint64  timestampMs = 0;
};

#endif