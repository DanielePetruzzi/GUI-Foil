#pragma once

#include <QPointF>
#include <QVector>
#include <QRectF>

class Geometry_Utils
{
public:

    // Distance

    static double distance(const QPointF& p1,
                           const QPointF& p2);

    static double squaredDistance(const QPointF& p1,
                                  const QPointF& p2);

    // Angles

    static double angle(const QPointF& p1,
                        const QPointF& p2);

    // Transformations

    static QPointF rotate(const QPointF& point,
                          double angleDeg);

    static QPointF translate(const QPointF& point,
                             double dx,
                             double dy);

    static QPointF scale(const QPointF& point,
                         double factor);

    // Vector

    static QPointF midpoint(const QPointF& p1,
                            const QPointF& p2);

    static QPointF interpolate(const QPointF& p1,
                               const QPointF& p2,
                               double t);

    // Polyline

    static double polylineLength(
        const QVector<QPointF>& points);

    static QRectF boundingRect(
        const QVector<QPointF>& points);
};