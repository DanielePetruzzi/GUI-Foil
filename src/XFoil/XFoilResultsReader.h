#pragma once

#include <QString>
#include <QVector>

namespace XFoil {

struct CpPoint {
    double x = 0.0, y = 0.0, cp = 0.0;
};

struct PolarPoint {
    double alpha  = 0.0;
    double cl     = 0.0;
    double cd     = 0.0;
    double cdp    = 0.0;
    double cm     = 0.0;
    double topXtr = 0.0;
    double botXtr = 0.0;
};

struct Coefficients {
    double cl = 0.0;
    double cm = 0.0;
    bool valid = false;
};

// Parses a Cp distribution file (x, y, Cp columns), skipping header lines.
QVector<CpPoint> loadCpDistribution(const QString &path);

// Parses an XFOIL polar file (alpha, CL, CD, CDp, CM[, Top_Xtr, Bot_Xtr]).
QVector<PolarPoint> loadPolar(const QString &path);

// Reads CL/CM from a stripped single-point accumulation file (Results.txt).
Coefficients readClCm(const QString &resultsPath);

} // namespace XFoil
