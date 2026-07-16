#pragma once

#include <QPointF>
#include <QPainter>
#include <QColor>
#include <QVector>
#include <QString>

// PlotTransform centralizes the world<->screen mapping that was previously
// duplicated (with small variations) across drawAirfoil, the morph canvas,
// the flap preview and the single-point solution plot.
//
// The caller is still responsible for computing the world bounds (including
// any padding rules specific to that plot), and passes them to fit(). The
// transform then owns scale/offset math and coordinate conversion.
class PlotTransform {
public:
    PlotTransform() = default;

    // Fits the world rectangle [minX,maxX] x [minY,maxY] into a
    // width x height area, uniformly scaled, with the given margin in
    // pixels on every side. World Y axis points up; screen Y axis points down.
    void fit(double minX, double maxX, double minY, double maxY,
             int width, int height, int margin);

    QPointF toScreen(const QPointF &worldPoint) const;
    QPointF toScreen(double x, double y) const;
    QPointF toWorld(const QPointF &screenPoint) const;

    double minX() const { return m_minX; }
    double maxX() const { return m_maxX; }
    double minY() const { return m_minY; }
    double maxY() const { return m_maxY; }
    double scale() const { return m_scale; }
    bool isValid() const { return m_valid; }

private:
    double m_minX = 0.0, m_maxX = 1.0;
    double m_minY = 0.0, m_maxY = 1.0;
    double m_scale = 1.0;
    double m_offsetX = 0.0, m_offsetY = 0.0;
    int    m_height = 0;
    bool   m_valid = false;
};

// Free helper functions shared by the simpler plots (airfoil view, morph
// canvas, flap preview): a plain 10-division background grid and a legend
// line entry. The more elaborate axis-labeled grid used by the XY polar
// plots stays local to that dialog, since its layout differs enough that
// forcing it through the same helper would hurt readability.
namespace PlotUtils {

void drawSimpleGrid(QPainter &painter, const PlotTransform &t, int divisions = 10);

// Draws one legend row (a short colored line followed by a label) and
// advances (x, y) to the next row position.
void drawLegendEntry(QPainter &painter, int &x, int &y,
                      const QColor &color, Qt::PenStyle style,
                      const QString &text, int lineLength = 22, int rowHeight = 18);

QVector<QPointF> mapPolyline(const PlotTransform &t, const QVector<QPointF> &worldPoints);

} // namespace PlotUtils
