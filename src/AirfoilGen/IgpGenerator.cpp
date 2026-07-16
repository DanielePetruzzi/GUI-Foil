#include "IgpGenerator.h"
#include "../Geometry/Bezier.h"
#include "../Math/LinearSolver.h"

#include <cmath>

namespace AirfoilGen {

IgpResult generatePoints(const IgpParams &p, int samples)
{
    IgpResult result;

    if (p.T <= 0.0) {
        result.errorMessage = "T must be greater than zero.";
        return result;
    }
    if (p.XT <= 0.0 || p.XT >= 1.0) {
        result.errorMessage = "XT must be between 0 and 1.";
        return result;
    }
    if (p.rho0Input < 0.0) {
        result.errorMessage = "rho0 must be greater than or equal to zero.";
        return result;
    }

    const int N = samples;

    QVector<double> x(N), yc(N), yu(N), yl(N);
    for (int i = 0; i < N; ++i)
        x[i] = double(i) / double(N - 1);

    for (int i = 0; i < N; ++i) {
        const double k = Geometry::findBezierParameter(x[i], p.c1, p.c2);
        yc[i] = Geometry::evaluateBezierY(k, p.c3, p.c4);
    }

    const double rho0   = p.rho0Input * p.T * p.T / (p.XT * p.XT);
    const double betaTE = p.betaInput * std::atan(p.T / (1.0 - p.XT));

    double G[5][5];
    const double XT = p.XT;

    G[0][0] = std::sqrt(XT);        G[0][1] = 0.5 / std::sqrt(XT);  G[0][2] = 0.25; G[0][3] = 1.0; G[0][4] = 1.0;
    G[1][0] = XT;                   G[1][1] = 1.0;                  G[1][2] = 0.50; G[1][3] = 0.0; G[1][4] = 1.0;
    G[2][0] = XT * XT;              G[2][1] = 2.0 * XT;             G[2][2] = 1.00; G[2][3] = 0.0; G[2][4] = 1.0;
    G[3][0] = XT * XT * XT;         G[3][1] = 3.0 * XT * XT;        G[3][2] = 1.50; G[3][3] = 0.0; G[3][4] = 1.0;
    G[4][0] = XT * XT * XT * XT;    G[4][1] = 4.0 * XT * XT * XT;   G[4][2] = 2.00; G[4][3] = 0.0; G[4][4] = 1.0;

    double GT[5][5];
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            GT[i][j] = G[j][i];

    double rhs[5];
    rhs[0] = p.T;
    rhs[1] = 0.0;
    rhs[2] = -std::tan(betaTE / 2.0);
    rhs[3] = std::sqrt(2.0 * rho0);
    rhs[4] = 0.0;

    const QVector<double> t = MathUtils::solve5x5(GT, rhs);

    for (int i = 0; i < N; ++i) {
        const double xx = x[i];
        const double yt = t[0] * std::sqrt(xx) + t[1] * xx + t[2] * xx * xx +
                           t[3] * xx * xx * xx + t[4] * xx * xx * xx * xx;

        yu[i] = yc[i] + 0.5 * yt;
        yl[i] = yc[i] - 0.5 * yt;
    }

    result.points.reserve(2 * N - 1);
    for (int i = N - 1; i >= 0; --i)
        result.points.append(QPointF(x[i], yu[i]));
    for (int i = 1; i < N; ++i)
        result.points.append(QPointF(x[i], yl[i]));

    result.valid = true;
    return result;
}

QString buildHeader(const IgpParams &p)
{
    return QString("IGP c1 %1 c2 %2 c3 %3 c4 %4 T %5 XT %6 rho0 %7 betaTE %8")
        .arg(p.c1).arg(p.c2).arg(p.c3).arg(p.c4)
        .arg(p.T).arg(p.XT).arg(p.rho0Input).arg(p.betaInput);
}

} // namespace AirfoilGen
