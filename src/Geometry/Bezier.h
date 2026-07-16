#pragma once

// Cubic Bezier utilities used by the IGP airfoil generator.
// The curve is defined with P0=(0,0), P1=(c1,c3), P2=(c2,c4), P3=(1,0):
//   X(k) = 3k(1-k)^2 * c1 + 3(1-k)k^2 * c2 + k^3
//   Y(k) = 3k(1-k)^2 * c3 + 3(1-k)k^2 * c4
namespace Geometry {

// Finds the parameter k in [0,1] such that X(k) == X, via bisection.
double findBezierParameter(double X, double c1, double c2, int iterations = 60);

// Evaluates Y(k) given the two Y control coefficients (c3, c4).
double evaluateBezierY(double k, double c3, double c4);

} // namespace Geometry
