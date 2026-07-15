#pragma once

#include <QVector>

class Linear_Solver
{
public:

    static bool solve(
        QVector<QVector<double>>& A,
        QVector<double>& b,
        QVector<double>& x,
        double tolerance = 1e-12);

private:

    static void swapRows(
        QVector<QVector<double>>& A,
        QVector<double>& b,
        int r1,
        int r2);
};