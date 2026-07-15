#include "Airfoil.h"

#include <QtMath>

Airfoil::Airfoil()
{
}

Airfoil::Airfoil(const QString& name)
    : m_name(name)
{
}

//------------------------------------------------------------
// Name
//------------------------------------------------------------

void Airfoil::setName(const QString& name)
{
    m_name = name;
}

const QString& Airfoil::name() const
{
    return m_name;
}

//------------------------------------------------------------
// Points
//------------------------------------------------------------

void Airfoil::clear()
{
    m_points.clear();
}

void Airfoil::reserve(int size)
{
    m_points.reserve(size);
}

void Airfoil::addPoint(const QPointF& point)
{
    m_points.append(point);
}

void Airfoil::addPoint(double x, double y)
{
    m_points.append(QPointF(x, y));
}

void Airfoil::insertPoint(int index, const QPointF& point)
{
    m_points.insert(index, point);
}

void Airfoil::removePoint(int index)
{
    m_points.remove(index);
}

void Airfoil::setPoint(int index, const QPointF& point)
{
    m_points[index] = point;
}

QPointF Airfoil::point(int index) const
{
    return m_points.at(index);
}

QPointF& Airfoil::operator[](int index)
{
    return m_points[index];
}

const QPointF& Airfoil::operator[](int index) const
{
    return m_points[index];
}

QVector<QPointF>& Airfoil::points()
{
    return m_points;
}

const QVector<QPointF>& Airfoil::points() const
{
    return m_points;
}

//------------------------------------------------------------
// Info
//------------------------------------------------------------

int Airfoil::size() const
{
    return m_points.size();
}

bool Airfoil::isEmpty() const
{
    return m_points.isEmpty();
}

QRectF Airfoil::boundingRect() const
{
    if (m_points.isEmpty())
        return QRectF();

    double minX = m_points.first().x();
    double maxX = minX;

    double minY = m_points.first().y();
    double maxY = minY;

    for (const QPointF& p : m_points)
    {
        minX = qMin(minX, p.x());
        maxX = qMax(maxX, p.x());

        minY = qMin(minY, p.y());
        maxY = qMax(maxY, p.y());
    }

    return QRectF(
        QPointF(minX, minY),
        QPointF(maxX, maxY)
    );
}

QPointF Airfoil::leadingEdge() const
{
    if (m_points.isEmpty())
        return QPointF();

    QPointF le = m_points.first();

    for (const QPointF& p : m_points)
    {
        if (p.x() < le.x())
            le = p;
    }

    return le;
}

QPointF Airfoil::trailingEdge() const
{
    if (m_points.isEmpty())
        return QPointF();

    QPointF te = m_points.first();

    for (const QPointF& p : m_points)
    {
        if (p.x() > te.x())
            te = p;
    }

    return te;
}

double Airfoil::chord() const
{
    return trailingEdge().x() - leadingEdge().x();
}

//------------------------------------------------------------
// Geometry
//------------------------------------------------------------

void Airfoil::translate(double dx, double dy)
{
    for (QPointF& p : m_points)
    {
        p.rx() += dx;
        p.ry() += dy;
    }
}

void Airfoil::scale(double factor)
{
    for (QPointF& p : m_points)
    {
        p.rx() *= factor;
        p.ry() *= factor;
    }
}

void Airfoil::rotate(double angleDeg)
{
    const double rad = qDegreesToRadians(angleDeg);

    const double c = qCos(rad);
    const double s = qSin(rad);

    for (QPointF& p : m_points)
    {
        const double x = p.x();
        const double y = p.y();

        p.setX(c * x - s * y);
        p.setY(s * x + c * y);
    }
}

void Airfoil::mirrorY()
{
    for (QPointF& p : m_points)
    {
        p.setY(-p.y());
    }
}