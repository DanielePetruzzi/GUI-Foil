#pragma once

#include <QVector>

namespace MathUtils {

// Solves the 5x5 linear system A*x = b using Gaussian elimination with
// partial pivoting. Returns a zero vector if the system is singular
// (pivot below tolerance), matching the original behavior.
QVector<double> solve5x5(double A[5][5], double b[5]);

// General N x N counterpart (used by the PARSEC generator, which needs
// two independent 6x6 systems). Same Gaussian elimination with partial
// pivoting; A must be square and match b's size.
QVector<double> solveNxN(const QVector<QVector<double>> &A, const QVector<double> &b);

} // namespace MathUtils
