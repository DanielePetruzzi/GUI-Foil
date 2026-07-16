#pragma once

#include <QWidget>
#include <QResizeEvent>
#include <functional>

// QWidget that invokes a callback whenever it is resized.
// Used as the airfoil plot area in MainWindow.
class ResizableWidget : public QWidget {
public:
    std::function<void()> onResize;

protected:
    void resizeEvent(QResizeEvent *e) override
    {
        QWidget::resizeEvent(e);
        if (onResize)
            onResize();
    }
};
