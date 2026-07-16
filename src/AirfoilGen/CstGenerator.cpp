#include "CstGenerator.h"

#include <QtMath>

namespace AirfoilGen {

QVector<QPointF> generatePoints(const CstParams &p, int samplesPerSurface)
{
    const int N = samplesPerSurface;

    QVector<double> x(N);
    for (int i = 0; i < N; ++i)
        x[i] = double(i) / (N - 1);

    QVector<double> yu(N), yl(N);
    for (int i = 0; i < N; ++i) {
        const double xi = x[i];
        const double Cx = qPow(xi, p.n1) * qPow(1.0 - xi, p.n2);

        double Su = 0.0, Sl = 0.0;
        for (int j = 0; j < 4; ++j) {
            Su += p.upper[j] * qPow(xi, j);
            Sl += p.lower[j] * qPow(xi, j);
        }

        yu[i] = Cx * Su;
        yl[i] = Cx * Sl;
    }

    QVector<QPointF> pts;
    pts.reserve(2 * N - 1);
    for (int i = N - 1; i >= 0; --i)
        pts.append(QPointF(x[i], yu[i]));
    for (int i = 1; i < N; ++i)
        pts.append(QPointF(x[i], yl[i]));

    return pts;
}

QString buildHeader(const CstParams &p)
{
    return QString("CST N1 %1 N2 %2 A1U %3 A2U %4 A3U %5 A4U %6 A1L %7 A2L %8 A3L %9 A4L %10")
        .arg(p.n1).arg(p.n2)
        .arg(p.upper[0]).arg(p.upper[1]).arg(p.upper[2]).arg(p.upper[3])
        .arg(p.lower[0]).arg(p.lower[1]).arg(p.lower[2]).arg(p.lower[3]);
}

} // namespace AirfoilGen
