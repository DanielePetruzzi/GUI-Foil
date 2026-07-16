#pragma once

#include <QLabel>
#include <QResizeEvent>
#include <functional>

// QLabel that invokes a callback with its new (width, height) whenever it
// is resized. Used as canvas for the morph tool, flap preview, and the
// various solution plots.
class ResizableLabel : public QLabel {
public:
    std::function<void(int, int)> onResize;

protected:
    void resizeEvent(QResizeEvent *e) override
    {
        QLabel::resizeEvent(e);
        if (onResize)
            onResize(width(), height());
    }
};
