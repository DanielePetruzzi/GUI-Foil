#include "Geometry_Utils.h"

#include <QtMath>

double Geometry_Utils::distance(
    const QPointF& p1,
    const QPointF& p2)
{
    return qSqrt(squaredDistance(p1, p2));
}

double Geometry_Utils::squaredDistance(
    const QPointF& p1,
    const QPointF& p2)
{
    const double dx = p2.x() - p1.x();
    const double dy = p2.y() - p1.y();

    return dx * dx + dy * dy;
}

double Geometry_Utils::angle(
    const QPointF& p1,
    const QPointF& p2)
{
    return qRadiansToDegrees(
        qAtan2(p2.y() - p1.y(),
               p2.x() - p1.x()));
}

QPointF Geometry_Utils::rotate(
    const QPointF& point,
    double angleDeg)
{
    const double a = qDegreesToRadians(angleDeg);

    const double c = qCos(a);
    const double s = qSin(a);

    return QPointF(
        c * point.x() - s * point.y(),
        s * point.x() + c * point.y());
}

QPointF Geometry_Utils::translate(
    const QPointF& point,
    double dx,
    double dy)
{
    return QPointF(
        point.x() + dx,
        point.y() + dy);
}

QPointF Geometry_Utils::scale(
    const QPointF& point,
    double factor)
{
    return QPointF(
        point.x() * factor,
        point.y() * factor);
}

QPointF Geometry_Utils::midpoint(
    const QPointF& p1,
    const QPointF& p2)
{
    return QPointF(
        (p1.x() + p2.x()) * 0.5,
        (p1.y() + p2.y()) * 0.5);
}

QPointF Geometry_Utils::interpolate(
    const QPointF& p1,
    const QPointF& p2,
    double t)
{
    return QPointF(
        p1.x() + (p2.x() - p1.x()) * t,
        p1.y() + (p2.y() - p1.y()) * t);
}

double Geometry_Utils::polylineLength(
    const QVector<QPointF>& points)
{
    if (points.size() < 2)
        return 0.0;

    double length = 0.0;

    for (int i = 1; i < points.size(); ++i)
        length += distance(points[i - 1], points[i]);

    return length;
}

QRectF Geometry_Utils::boundingRect(
    const QVector<QPointF>& points)
{
    if (points.isEmpty())
        return QRectF();

    double minX = points.first().x();
    double maxX = minX;

    double minY = points.first().y();
    double maxY = minY;

    for (const QPointF& p : points)
    {
        minX = qMin(minX, p.x());
        maxX = qMax(maxX, p.x());

        minY = qMin(minY, p.y());
        maxY = qMax(maxY, p.y());
    }

    return QRectF(
        QPointF(minX, minY),
        QPointF(maxX, maxY));
}