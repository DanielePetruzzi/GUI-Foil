#include "Resizable_Label.h"

#include <QPainter>
#include <QPaintEvent>

Resizable_Label::Resizable_Label(QWidget* parent)
    : Resizable_Widget(parent)
{
}

void Resizable_Label::clear()
{
    m_originalPixmap = QPixmap();
    m_scaledPixmap = QPixmap();

    update();
}

void Resizable_Label::setPixmap(const QPixmap& pixmap)
{
    m_originalPixmap = pixmap;

    updateScaledPixmap();

    update();
}

const QPixmap& Resizable_Label::pixmap() const
{
    return m_originalPixmap;
}

bool Resizable_Label::hasPixmap() const
{
    return !m_originalPixmap.isNull();
}

void Resizable_Label::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.fillRect(rect(), palette().window());

    if (m_scaledPixmap.isNull())
        return;

    const int x = (width() - m_scaledPixmap.width()) / 2;
    const int y = (height() - m_scaledPixmap.height()) / 2;

    painter.drawPixmap(x, y, m_scaledPixmap);
}

void Resizable_Label::onResize(const QSize&)
{
    updateScaledPixmap();
}

void Resizable_Label::updateScaledPixmap()
{
    if (m_originalPixmap.isNull())
    {
        m_scaledPixmap = QPixmap();
        return;
    }

    m_scaledPixmap =
        m_originalPixmap.scaled(
            size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation);
}