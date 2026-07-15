#include "Resizable_Widget.h"

#include <QResizeEvent>

Resizable_Widget::Resizable_Widget(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding,
                  QSizePolicy::Expanding);
}

QSize Resizable_Widget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize Resizable_Widget::sizeHint() const
{
    return QSize(400, 300);
}

void Resizable_Widget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    onResize(event->size());

    emit resized(event->size());
}

void Resizable_Widget::onResize(const QSize&)
{
    // Implementabile dalle classi derivate.
}