#include "Plot_Renderer.h"
#include "Plot_Colors.h"

#include "../Core/Airfoil.h"

#include <QPainterPath>

Plot_Renderer::Plot_Renderer()
{
}

void Plot_Renderer::setViewport(const QRectF& viewport)
{
    m_viewport = viewport;
}

void Plot_Renderer::setWorld(const QRectF& world)
{
    m_world = world;
}

QRectF Plot_Renderer::viewport() const
{
    return m_viewport;
}

QRectF Plot_Renderer::world() const
{
    return m_world;
}

QPointF Plot_Renderer::worldToScreen(const QPointF& point) const
{
    if (m_world.width() == 0.0 || m_world.height() == 0.0)
        return QPointF();

    const double x =
        (point.x() - m_world.left()) *
        m_viewport.width() /
        m_world.width();

    const double y =
        (m_world.bottom() - point.y()) *
        m_viewport.height() /
        m_world.height();

    return QPointF(
        m_viewport.left() + x,
        m_viewport.top() + y);
}

QPointF Plot_Renderer::screenToWorld(const QPointF& point) const
{
    if (m_viewport.width() == 0.0 || m_viewport.height() == 0.0)
        return QPointF();

    const double x =
        m_world.left() +
        (point.x() - m_viewport.left()) *
        m_world.width() /
        m_viewport.width();

    const double y =
        m_world.bottom() -
        (point.y() - m_viewport.top()) *
        m_world.height() /
        m_viewport.height();

    return QPointF(x, y);
}

void Plot_Renderer::drawGrid(QPainter& painter) const
{
    painter.save();

    painter.setPen(Plot_Colors::gridPen());
    
    constexpr int divisions = 10;

    for(int i=0;i<=divisions;++i)
    {
        const double x =
            m_viewport.left() +
            i * m_viewport.width() / divisions;

        painter.drawLine(
            QPointF(x,m_viewport.top()),
            QPointF(x,m_viewport.bottom()));

        const double y =
            m_viewport.top() +
            i * m_viewport.height() / divisions;

        painter.drawLine(
            QPointF(m_viewport.left(),y),
            QPointF(m_viewport.right(),y));
    }

    painter.restore();
}

void Plot_Renderer::drawAxes(QPainter& painter) const
{
    painter.save();

    painter.setPen(Plot_Colors::axesPen());

    const QPointF x1 =
        worldToScreen(QPointF(m_world.left(),0.0));

    const QPointF x2 =
        worldToScreen(QPointF(m_world.right(),0.0));

    painter.drawLine(x1,x2);

    const QPointF y1 =
        worldToScreen(QPointF(0.0,m_world.top()));

    const QPointF y2 =
        worldToScreen(QPointF(0.0,m_world.bottom()));

    painter.drawLine(y1,y2);

    painter.restore();
}

void Plot_Renderer::drawPolyline(
    QPainter& painter,
    const QVector<QPointF>& points,
    const QPen& pen) const
{
    if(points.size()<2)
        return;

    painter.save();

    painter.setPen(pen);

    QPainterPath path;

    path.moveTo(worldToScreen(points.first()));

    for(int i=1;i<points.size();++i)
        path.lineTo(worldToScreen(points[i]));

    painter.drawPath(path);

    painter.restore();
}

void Plot_Renderer::drawAirfoil(
    QPainter& painter,
    const Airfoil& airfoil,
    const QPen& pen) const
{
    drawPolyline(
        painter,
        airfoil.points(),
        pen);
}

void Plot_Renderer::drawPoints(
    QPainter& painter,
    const QVector<QPointF>& points,
    int radius,
    const QBrush& brush) const
{
    painter.save();

    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);

    for(const QPointF& p : points)
    {
        const QPointF s =
            worldToScreen(p);

        painter.drawEllipse(
            s,
            radius,
            radius);
    }

    painter.restore();
}