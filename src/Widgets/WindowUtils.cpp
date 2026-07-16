#include "WindowUtils.h"

#include <QMouseEvent>
#include <QTimer>
#include <QtGlobal>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace WindowUtils {

QPoint mouseEventPos(QMouseEvent *event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return event->position().toPoint();
#else
    return event->pos();
#endif
}

void bringWindowToFront(QWidget *w)
{
    if (!w)
        return;

    if (w->isMinimized())
        w->showNormal();

#ifdef Q_OS_WIN
    const Qt::WindowFlags originalFlags = w->windowFlags();
    w->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    w->show();
    w->raise();
    w->activateWindow();

    QTimer::singleShot(200, w, [w, originalFlags]() {
        w->setWindowFlags(originalFlags);
        w->show();
        w->raise();
        w->activateWindow();
    });
#else
    w->show();
    w->raise();
    w->activateWindow();
#endif
}

} // namespace WindowUtils
