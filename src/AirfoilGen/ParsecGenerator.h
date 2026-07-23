#pragma once

#include <QString>
#include <QVector>
#include <QPointF>

// PARSEC airfoil parameterization (Sobieczky, 1997).
//
// Each surface is an analytic polynomial in half-integer powers of x:
//   z(x) = sum_{n=1..6} a_n * x^(n - 0.5)
// which lets the leading-edge radius, crest position/curvature and
// trailing-edge geometry be imposed directly as boundary conditions,
// instead of free coefficients as in CST.
namespace AirfoilGen {

struct ParsecParams {
    double rle = 0.01;        // leading edge radius

    double xUp  = 0.30;       // upper crest x position
    double zUp  = 0.06;       // upper crest z (thickness/camber height)
    double zxxUp = -0.40;     // upper crest curvature (d2z/dx2)

    double xLo  = 0.30;       // lower crest x position
    double zLo  = -0.06;      // lower crest z
    double zxxLo = 0.40;      // lower crest curvature (d2z/dx2)

    double zte      = 0.0;    // trailing edge mean z (camber at TE)
    double deltaZte = 0.0;    // trailing edge thickness (upper - lower)
    double alphaTe  = 0.0;    // trailing edge direction angle (deg)
    double betaTe   = 10.0;   // trailing edge wedge angle (deg)
};

struct ParsecResult {
    bool valid = false;
    QString errorMessage;      // set when valid == false
    QVector<QPointF> points;   // set when valid == true
};

// Validates params and, if valid, solves the two 6x6 coefficient systems
// and samples the resulting outline (cosine spacing, denser near the
// leading edge).
ParsecResult generatePoints(const ParsecParams &params, int samplesPerSurface = 150);

QString buildHeader(const ParsecParams &params);

} // namespace AirfoilGen
