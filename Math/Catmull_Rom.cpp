#include "Catmull_Rom.h"

QVector<QPointF> Catmull_Rom::interpolate(
    const QVector<QPointF>& controlPoints,
    int samplesPerSegment)
{
    QVector<QPointF> result;

    if (controlPoints.size() < 2)
        return result;

    if (samplesPerSegment < 2)
        samplesPerSegment = 2;

    for (int i = 0; i < controlPoints.size() - 1; ++i)
    {
        const QPointF& p0 =
            (i == 0) ? controlPoints[i] : controlPoints[i - 1];

        const QPointF& p1 = controlPoints[i];

        const QPointF& p2 = controlPoints[i + 1];

        const QPointF& p3 =
            (i + 2 < controlPoints.size())
                ? controlPoints[i + 2]
                : controlPoints[i + 1];

        for (int j = 0; j < samplesPerSegment; ++j)
        {
            const double t =
                static_cast<double>(j) /
                static_cast<double>(samplesPerSegment);

            result.append(
                interpolatePoint(p0, p1, p2, p3, t));
        }
    }

    result.append(controlPoints.last());

    return result;
}

QPointF Catmull_Rom::interpolatePoint(
    const QPointF& p0,
    const QPointF& p1,
    const QPointF& p2,
    const QPointF& p3,
    double t)
{
    const double t2 = t * t;
    const double t3 = t2 * t;

    const double x =
        0.5 * ((2.0 * p1.x()) +
               (-p0.x() + p2.x()) * t +
               (2.0 * p0.x() - 5.0 * p1.x() + 4.0 * p2.x() - p3.x()) * t2 +
               (-p0.x() + 3.0 * p1.x() - 3.0 * p2.x() + p3.x()) * t3);

    const double y =
        0.5 * ((2.0 * p1.y()) +
               (-p0.y() + p2.y()) * t +
               (2.0 * p0.y() - 5.0 * p1.y() + 4.0 * p2.y() - p3.y()) * t2 +
               (-p0.y() + 3.0 * p1.y() - 3.0 * p2.y() + p3.y()) * t3);

    return QPointF(x, y);
}