#include "LinearSolver.h"

#include <cmath>
#include <algorithm>

namespace MathUtils {

QVector<double> solve5x5(double A[5][5], double b[5])
{
    double M[5][6];

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j)
            M[i][j] = A[i][j];
        M[i][5] = b[i];
    }

    for (int k = 0; k < 5; ++k) {
        int pivot = k;
        for (int i = k + 1; i < 5; ++i) {
            if (std::fabs(M[i][k]) > std::fabs(M[pivot][k]))
                pivot = i;
        }

        if (pivot != k) {
            for (int j = k; j < 6; ++j)
                std::swap(M[k][j], M[pivot][j]);
        }

        double diag = M[k][k];
        if (std::fabs(diag) < 1e-14)
            return QVector<double>(5, 0.0);

        for (int j = k; j < 6; ++j)
            M[k][j] /= diag;

        for (int i = k + 1; i < 5; ++i) {
            double factor = M[i][k];
            for (int j = k; j < 6; ++j)
                M[i][j] -= factor * M[k][j];
        }
    }

    QVector<double> x(5, 0.0);
    for (int i = 4; i >= 0; --i) {
        x[i] = M[i][5];
        for (int j = i + 1; j < 5; ++j)
            x[i] -= M[i][j] * x[j];
    }

    return x;
}

QVector<double> solveNxN(const QVector<QVector<double>> &A, const QVector<double> &b)
{
    const int n = b.size();
    QVector<QVector<double>> M(n, QVector<double>(n + 1, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            M[i][j] = A[i][j];
        M[i][n] = b[i];
    }

    for (int k = 0; k < n; ++k) {
        int pivot = k;
        for (int i = k + 1; i < n; ++i) {
            if (std::fabs(M[i][k]) > std::fabs(M[pivot][k]))
                pivot = i;
        }

        if (pivot != k)
            std::swap(M[k], M[pivot]);

        double diag = M[k][k];
        if (std::fabs(diag) < 1e-14)
            return QVector<double>(n, 0.0);

        for (int j = k; j <= n; ++j)
            M[k][j] /= diag;

        for (int i = k + 1; i < n; ++i) {
            double factor = M[i][k];
            for (int j = k; j <= n; ++j)
                M[i][j] -= factor * M[k][j];
        }
    }

    QVector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = M[i][n];
        for (int j = i + 1; j < n; ++j)
            x[i] -= M[i][j] * x[j];
    }

    return x;
}

} // namespace MathUtils
