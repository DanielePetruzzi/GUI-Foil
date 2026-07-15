#pragma once

#include <QColor>
#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QVector>

class Airfoil;

class Plot_Renderer
{
public:

    Plot_Renderer();

    void setViewport(const QRectF& viewport);

    void setWorld(const QRectF& world);

    QRectF viewport() const;

    QRectF world() const;

    QPointF worldToScreen(const QPointF& point) const;

    QPointF screenToWorld(const QPointF& point) const;

    void drawGrid(QPainter& painter) const;

    void drawAxes(QPainter& painter) const;

    void drawPolyline(QPainter& painter,
                      const QVector<QPointF>& points,
                      const QPen& pen) const;

    void drawAirfoil(QPainter& painter,
                     const Airfoil& airfoil,
                     const QPen& pen) const;

    void drawPoints(QPainter& painter,
                    const QVector<QPointF>& points,
                    int radius,
                    const QBrush& brush) const;

private:

    QRectF m_viewport;
    QRectF m_world;
};