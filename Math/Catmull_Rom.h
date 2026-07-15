#pragma once

#include <QPointF>
#include <QVector>

class Catmull_Rom
{
public:

    Catmull_Rom() = default;
    explicit Catmull_Rom(const QVector<QPointF>& controlPoints);

    void setControlPoints(const QVector<QPointF>& controlPoints);

    const QVector<QPointF>& controlPoints() const;

    bool isValid() const;

    QVector<QPointF> sample(int samplesPerSegment = 20) const;

private:

    static QPointF interpolate(
        const QPointF& p0,
        const QPointF& p1,
        const QPointF& p2,
        const QPointF& p3,
        double t);

private:

    QVector<QPointF> m_controlPoints;
};