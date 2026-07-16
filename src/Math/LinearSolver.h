#pragma once

#include <QVector>

namespace MathUtils {

// Solves the 5x5 linear system A*x = b using Gaussian elimination with
// partial pivoting. Returns a zero vector if the system is singular
// (pivot below tolerance), matching the original behavior.
QVector<double> solve5x5(double A[5][5], double b[5]);

} // namespace MathUtils
