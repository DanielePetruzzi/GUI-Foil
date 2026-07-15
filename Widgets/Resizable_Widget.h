#pragma once

#include <QWidget>

class Resizable_Widget : public QWidget
{
    Q_OBJECT

public:

    explicit Resizable_Widget(QWidget* parent = nullptr);
    virtual ~Resizable_Widget() = default;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

signals:

    void resized(const QSize& size);

protected:

    virtual void onResize(const QSize& newSize);

    void resizeEvent(QResizeEvent* event) override;
};