#pragma once

#include <QString>
#include <QVector>
#include <QPointF>

namespace AirfoilGen {

struct CstParams {
    double n1 = 0.0, n2 = 0.0;
    double upper[4] = { 0.0, 0.0, 0.0, 0.0 }; // A1U..A4U
    double lower[4] = { 0.0, 0.0, 0.0, 0.0 }; // A1L..A4L
};

// Computes the airfoil outline (upper surface trailing-edge to leading edge,
// then leading edge to lower surface trailing edge) using the CST
// class/shape function method. samplesPerSurface controls the resolution
// of each surface (200 in the original tool).
QVector<QPointF> generatePoints(const CstParams &params, int samplesPerSurface = 200);

// Builds the .dat header line encoding the CST parameters.
QString buildHeader(const CstParams &params);

} // namespace AirfoilGen
