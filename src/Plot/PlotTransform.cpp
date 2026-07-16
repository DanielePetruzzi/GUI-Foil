#include "PlotTransform.h"
#include "PlotColors.h"

#include <QtMath>
#include <QFont>

void PlotTransform::fit(double minX, double maxX, double minY, double maxY,
                         int width, int height, int margin)
{
    m_minX = minX; m_maxX = maxX;
    m_minY = minY; m_maxY = maxY;
    m_height = height;

    if (width <= 0 || height <= 0 || maxX <= minX || maxY <= minY) {
        m_valid = false;
        return;
    }

    const double scaleX = (width  - 2 * margin) / (maxX - minX);
    const double scaleY = (height - 2 * margin) / (maxY - minY);
    m_scale = qMin(scaleX, scaleY);

    m_offsetX = margin + (width  - 2 * margin - (maxX - minX) * m_scale) / 2.0;
    m_offsetY = margin + (height - 2 * margin - (maxY - minY) * m_scale) / 2.0;

    m_valid = true;
}

QPointF PlotTransform::toScreen(const QPointF &worldPoint) const
{
    return toScreen(worldPoint.x(), worldPoint.y());
}

QPointF PlotTransform::toScreen(double x, double y) const
{
    return QPointF(m_offsetX + (x - m_minX) * m_scale,
                   m_height - m_offsetY - (y - m_minY) * m_scale);
}

QPointF PlotTransform::toWorld(const QPointF &screenPoint) const
{
    return QPointF((screenPoint.x() - m_offsetX) / m_scale + m_minX,
                    (m_height - screenPoint.y() - m_offsetY) / m_scale + m_minY);
}

namespace PlotUtils {

void drawSimpleGrid(QPainter &painter, const PlotTransform &t, int divisions)
{
    if (!t.isValid())
        return;

    painter.setPen(QPen(PlotColors::grid, 0.5));

    const double dx = (t.maxX() - t.minX()) / divisions;
    const double dy = (t.maxY() - t.minY()) / divisions;

    for (int i = 0; i <= divisions; ++i) {
        const double gx = t.minX() + i * dx;
        const double gy = t.minY() + i * dy;
        painter.drawLine(t.toScreen(gx, t.maxY()), t.toScreen(gx, t.minY()));
        painter.drawLine(t.toScreen(t.minX(), gy), t.toScreen(t.maxX(), gy));
    }
}

void drawLegendEntry(QPainter &painter, int &x, int &y,
                      const QColor &color, Qt::PenStyle style,
                      const QString &text, int lineLength, int rowHeight)
{
    painter.setPen(QPen(color, 1.8, style));
    painter.drawLine(x, y, x + lineLength, y);

    painter.setPen(PlotColors::gridText);
    painter.setFont(QFont("Arial", 8));
    painter.drawText(x + lineLength + 5, y + 4, text);

    y += rowHeight;
}

QVector<QPointF> mapPolyline(const PlotTransform &t, const QVector<QPointF> &worldPoints)
{
    QVector<QPointF> screenPoints;
    screenPoints.reserve(worldPoints.size());
    for (const QPointF &p : worldPoints)
        screenPoints.append(t.toScreen(p));
    return screenPoints;
}

} // namespace PlotUtils
