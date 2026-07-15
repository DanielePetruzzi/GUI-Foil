#pragma once

#include <QPointF>
#include <QVector>

class Catmull_Rom
{
public:

    static QVector<QPointF> interpolate(
        const QVector<QPointF>& controlPoints,
        int samplesPerSegment = 20);

private:

    static QPointF interpolatePoint(
        const QPointF& p0,
        const QPointF& p1,
        const QPointF& p2,
        const QPointF& p3,
        double t);
};