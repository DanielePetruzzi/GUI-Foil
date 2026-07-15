#include "Catmull_Rom.h"

Catmull_Rom::Catmull_Rom(
    const QVector<QPointF>& controlPoints)
    : m_controlPoints(controlPoints)
{
}

void Catmull_Rom::setControlPoints(
    const QVector<QPointF>& controlPoints)
{
    m_controlPoints = controlPoints;
}

const QVector<QPointF>& Catmull_Rom::controlPoints() const
{
    return m_controlPoints;
}

bool Catmull_Rom::isValid() const
{
    return m_controlPoints.size() >= 2;
}

QVector<QPointF> Catmull_Rom::sample(
    int samplesPerSegment) const
{
    QVector<QPointF> result;

    if (!isValid())
        return result;

    if (samplesPerSegment < 2)
        samplesPerSegment = 2;

    result.reserve(
        (m_controlPoints.size() - 1) * samplesPerSegment + 1);

    for (int i = 0; i < m_controlPoints.size() - 1; ++i)
    {
        const QPointF& p0 =
            (i == 0) ? m_controlPoints[i]
                     : m_controlPoints[i - 1];

        const QPointF& p1 = m_controlPoints[i];

        const QPointF& p2 = m_controlPoints[i + 1];

        const QPointF& p3 =
            (i + 2 < m_controlPoints.size())
                ? m_controlPoints[i + 2]
                : m_controlPoints[i + 1];

        const double step =
            1.0 / static_cast<double>(samplesPerSegment);

        for (int j = 0; j < samplesPerSegment; ++j)
        {
            result.append(
                interpolate(
                    p0,
                    p1,
                    p2,
                    p3,
                    j * step));
        }
    }

    result.append(m_controlPoints.last());

    return result;
}

QPointF Catmull_Rom::interpolate(
    const QPointF& p0,
    const QPointF& p1,
    const QPointF& p2,
    const QPointF& p3,
    double t)
{
    const double t2 = t * t;
    const double t3 = t2 * t;

    const double x =
        0.5 *
        ((2.0 * p1.x()) +
         (-p0.x() + p2.x()) * t +
         (2.0 * p0.x() - 5.0 * p1.x() + 4.0 * p2.x() - p3.x()) * t2 +
         (-p0.x() + 3.0 * p1.x() - 3.0 * p2.x() + p3.x()) * t3);

    const double y =
        0.5 *
        ((2.0 * p1.y()) +
         (-p0.y() + p2.y()) * t +
         (2.0 * p0.y() - 5.0 * p1.y() + 4.0 * p2.y() - p3.y()) * t2 +
         (-p0.y() + 3.0 * p1.y() - 3.0 * p2.y() + p3.y()) * t3);

    return QPointF(x, y);
}