#pragma once

#include "Resizable_Widget.h"

#include <QPixmap>

class Resizable_Label : public Resizable_Widget
{
    Q_OBJECT

public:

    explicit Resizable_Label(QWidget* parent = nullptr);

    void clear();

    void setPixmap(const QPixmap& pixmap);

    const QPixmap& pixmap() const;

    bool hasPixmap() const;

protected:

    void paintEvent(QPaintEvent* event) override;

    void onResize(const QSize& newSize) override;

private:

    void updateScaledPixmap();

private:

    QPixmap m_originalPixmap;
    QPixmap m_scaledPixmap;
};