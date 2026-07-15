#include "Linear_Solver.h"

#include <QtMath>

void Linear_Solver::swapRows(
    QVector<QVector<double>>& A,
    QVector<double>& b,
    int r1,
    int r2)
{
    A.swapItemsAt(r1, r2);
    qSwap(b[r1], b[r2]);
}

bool Linear_Solver::solve(
    QVector<QVector<double>>& A,
    QVector<double>& b,
    QVector<double>& x,
    double tolerance)
{
    const int n = A.size();

    if (n == 0)
        return false;

    x.resize(n);

    for (int i = 0; i < n; ++i)
    {
        int pivot = i;

        for (int j = i + 1; j < n; ++j)
        {
            if (qAbs(A[j][i]) > qAbs(A[pivot][i]))
                pivot = j;
        }

        if (qAbs(A[pivot][i]) < tolerance)
            return false;

        if (pivot != i)
            swapRows(A, b, i, pivot);

        const double diag = A[i][i];

        for (int j = i; j < n; ++j)
            A[i][j] /= diag;

        b[i] /= diag;

        for (int k = 0; k < n; ++k)
        {
            if (k == i)
                continue;

            const double factor = A[k][i];

            for (int j = i; j < n; ++j)
                A[k][j] -= factor * A[i][j];

            b[k] -= factor * b[i];
        }
    }

    x = b;

    return true;
}