#include "Bezier.h"

#include <cmath>

namespace Geometry {

double findBezierParameter(double X, double c1, double c2, int iterations)
{
    double a = 0.0;
    double b = 1.0;

    for (int iter = 0; iter < iterations; ++iter) {
        double k = 0.5 * (a + b);

        double bx =
            3.0 * k * std::pow(1.0 - k, 2.0) * c1 +
            3.0 * (1.0 - k) * k * k * c2 +
            k * k * k;

        if (bx > X)
            b = k;
        else
            a = k;
    }

    return 0.5 * (a + b);
}

double evaluateBezierY(double k, double c3, double c4)
{
    return 3.0 * k * std::pow(1.0 - k, 2.0) * c3 +
           3.0 * (1.0 - k) * k * k * c4;
}

} // namespace Geometry
