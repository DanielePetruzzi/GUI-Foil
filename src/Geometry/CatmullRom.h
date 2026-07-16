#pragma once

#include <QVector>
#include <QPointF>

namespace Geometry {

// Samples a Catmull-Rom spline through the given control points.
// nSamples is the approximate total number of samples across the whole
// curve (distributed evenly across the segments), matching the behavior
// used by the airfoil morphing tool.
QVector<QPointF> catmullRomSpline(const QVector<QPointF> &controlPoints, int nSamples);

} // namespace Geometry
