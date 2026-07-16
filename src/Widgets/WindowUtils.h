#pragma once

#include <QPoint>
#include <QWidget>

class QMouseEvent;

namespace WindowUtils {

// Qt5/Qt6-compatible way to read a QMouseEvent's position.
QPoint mouseEventPos(QMouseEvent *event);

// Brings a (possibly minimized, possibly background) window to the front.
// On Windows this briefly toggles WindowStaysOnTopHint since raise()/
// activateWindow() alone are not always sufficient.
void bringWindowToFront(QWidget *w);

} // namespace WindowUtils
