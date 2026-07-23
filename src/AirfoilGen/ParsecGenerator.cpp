#include "ParsecGenerator.h"
#include "../Math/LinearSolver.h"

#include <QtMath>
#include <cmath>

namespace AirfoilGen {
namespace {

// Builds and solves the 6x6 system for one surface (upper or lower).
// leRadiusCoeff is +sqrt(2*rle) for the upper surface and -sqrt(2*rle)
// for the lower one. Returns the 6 coefficients a1..a6 of
//   z(x) = sum_{n=1..6} a_n * x^(n - 0.5)
QVector<double> solveSurface(double leRadiusCoeff,
                              double xCrest, double zCrest, double zxxCrest,
                              double zTE, double slopeTE)
{
    QVector<QVector<double>> A(6, QVector<double>(6, 0.0));
    QVector<double> b(6, 0.0);

    // n = 1..6, exponent en = n - 0.5
    // Row 0: leading-edge radius -> a1 = leRadiusCoeff
    A[0][0] = 1.0;
    b[0] = leRadiusCoeff;

    // Row 1: crest height
    for (int n = 1; n <= 6; ++n)
        A[1][n - 1] = std::pow(xCrest, n - 0.5);
    b[1] = zCrest;

    // Row 2: crest slope = 0
    for (int n = 1; n <= 6; ++n)
        A[2][n - 1] = (n - 0.5) * std::pow(xCrest, n - 1.5);
    b[2] = 0.0;

    // Row 3: crest curvature
    for (int n = 1; n <= 6; ++n)
        A[3][n - 1] = (n - 0.5) * (n - 1.5) * std::pow(xCrest, n - 2.5);
    b[3] = zxxCrest;

    // Row 4: trailing edge height (x = 1)
    for (int n = 1; n <= 6; ++n)
        A[4][n - 1] = 1.0;
    b[4] = zTE;

    // Row 5: trailing edge slope (x = 1)
    for (int n = 1; n <= 6; ++n)
        A[5][n - 1] = (n - 0.5);
    b[5] = slopeTE;

    return MathUtils::solveNxN(A, b);
}

double evaluateSurface(const QVector<double> &coeffs, double x)
{
    double z = 0.0;
    for (int n = 1; n <= 6; ++n)
        z += coeffs[n - 1] * std::pow(x, n - 0.5);
    return z;
}

} // namespace

ParsecResult generatePoints(const ParsecParams &p, int samplesPerSurface)
{
    ParsecResult result;

    if (p.rle <= 0.0) {
        result.errorMessage = "rle must be greater than zero.";
        return result;
    }
    if (p.xUp <= 0.0 || p.xUp >= 1.0) {
        result.errorMessage = "xUp must be between 0 and 1.";
        return result;
    }
    if (p.xLo <= 0.0 || p.xLo >= 1.0) {
        result.errorMessage = "xLo must be between 0 and 1.";
        return result;
    }

    const double leCoeff = std::sqrt(2.0 * p.rle);
    const double alphaRad = p.alphaTe * M_PI / 180.0;
    const double betaRad  = p.betaTe  * M_PI / 180.0;

    const double slopeUp = std::tan(alphaRad - betaRad / 2.0);
    const double slopeLo = std::tan(alphaRad + betaRad / 2.0);

    const QVector<double> upperCoeffs = solveSurface(
        leCoeff, p.xUp, p.zUp, p.zxxUp, p.zte + p.deltaZte / 2.0, slopeUp);

    const QVector<double> lowerCoeffs = solveSurface(
        -leCoeff, p.xLo, p.zLo, p.zxxLo, p.zte - p.deltaZte / 2.0, slopeLo);

    const int N = samplesPerSurface;
    QVector<double> x(N);
    for (int i = 0; i < N; ++i) {
        // cosine spacing: denser near the leading edge (x = 0)
        const double theta = M_PI * double(i) / double(N - 1);
        x[i] = 0.5 * (1.0 - std::cos(theta));
    }

    QVector<double> yu(N), yl(N);
    for (int i = 0; i < N; ++i) {
        yu[i] = evaluateSurface(upperCoeffs, x[i]);
        yl[i] = evaluateSurface(lowerCoeffs, x[i]);
    }

    result.points.reserve(2 * N - 1);
    for (int i = N - 1; i >= 0; --i)
        result.points.append(QPointF(x[i], yu[i]));
    for (int i = 1; i < N; ++i)
        result.points.append(QPointF(x[i], yl[i]));

    result.valid = true;
    return result;
}

QString buildHeader(const ParsecParams &p)
{
    return QString("PARSEC rle %1 xUp %2 zUp %3 zxxUp %4 xLo %5 zLo %6 zxxLo %7 zte %8 dzte %9 aTE %10 bTE %11")
        .arg(p.rle).arg(p.xUp).arg(p.zUp).arg(p.zxxUp)
        .arg(p.xLo).arg(p.zLo).arg(p.zxxLo)
        .arg(p.zte).arg(p.deltaZte).arg(p.alphaTe).arg(p.betaTe);
}

} // namespace AirfoilGen
