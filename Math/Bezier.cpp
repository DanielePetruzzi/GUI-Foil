#include "Bezier.h"

#include <QtMath>

Bezier::Bezier(const QVector<QPointF>& controlPoints)
    : m_controlPoints(controlPoints)
{
}

void Bezier::setControlPoints(const QVector<QPointF>& controlPoints)
{
    m_controlPoints = controlPoints;
}

const QVector<QPointF>& Bezier::controlPoints() const
{
    return m_controlPoints;
}

bool Bezier::isValid() const
{
    return m_controlPoints.size() >= 2;
}

int Bezier::degree() const
{
    return qMax(0, m_controlPoints.size() - 1);
}

QPointF Bezier::evaluate(double t) const
{
    if (m_controlPoints.isEmpty())
        return QPointF();

    QVector<QPointF> tmp = m_controlPoints;

    const int n = tmp.size();

    for (int k = 1; k < n; ++k)
    {
        for (int i = 0; i < n - k; ++i)
        {
            tmp[i].setX(
                (1.0 - t) * tmp[i].x() + t * tmp[i + 1].x());

            tmp[i].setY(
                (1.0 - t) * tmp[i].y() + t * tmp[i + 1].y());
        }
    }

    return tmp.first();
}

QPointF Bezier::derivative(double t) const
{
    const int n = degree();

    if (n < 1)
        return QPointF();

    QVector<QPointF> derivativePoints;
    derivativePoints.reserve(n);

    for (int i = 0; i < n; ++i)
    {
        derivativePoints.append(QPointF(
            n * (m_controlPoints[i + 1].x() - m_controlPoints[i].x()),
            n * (m_controlPoints[i + 1].y() - m_controlPoints[i].y())));
    }

    Bezier derivativeCurve(derivativePoints);

    return derivativeCurve.evaluate(t);
}

QVector<QPointF> Bezier::sample(int samples) const
{
    QVector<QPointF> curve;

    if (!isValid())
        return curve;

    if (samples < 2)
        samples = 2;

    curve.reserve(samples);

    const double step = 1.0 / static_cast<double>(samples - 1);

    for (int i = 0; i < samples; ++i)
        curve.append(evaluate(i * step));

    return curve;
}