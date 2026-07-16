#pragma once

#include <QString>
#include <QVector>
#include <QPointF>

namespace AirfoilGen {

struct IgpParams {
    double c1 = 0.30, c2 = 0.70;   // camber Bezier X control params
    double c3 = 0.05, c4 = 0.00;   // camber Bezier Y control params
    double T  = 0.12;              // max thickness
    double XT = 0.30;              // thickness location
    double rho0Input = 1.0;        // leading edge radius factor
    double betaInput = 1.0;        // trailing edge angle factor
};

struct IgpResult {
    bool valid = false;
    QString errorMessage;          // set when valid == false
    QVector<QPointF> points;       // set when valid == true
};

// Validates params and, if valid, computes the airfoil outline via the
// Bezier camber line + 5x5 thickness-distribution linear system.
IgpResult generatePoints(const IgpParams &params, int samples = 200);

QString buildHeader(const IgpParams &params);

} // namespace AirfoilGen
