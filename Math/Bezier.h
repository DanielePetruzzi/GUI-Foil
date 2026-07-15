#pragma once

#include <QPointF>
#include <QVector>

class Bezier
{
public:

    Bezier() = default;
    explicit Bezier(const QVector<QPointF>& controlPoints);

    void setControlPoints(const QVector<QPointF>& controlPoints);

    const QVector<QPointF>& controlPoints() const;

    bool isValid() const;

    int degree() const;

    QPointF evaluate(double t) const;

    QPointF derivative(double t) const;

    QVector<QPointF> sample(int samples) const;

private:

    QVector<QPointF> m_controlPoints;
};