#include "ChildDialogFactory.h"

#include <QtGlobal>

namespace Dialogs {

void setupChildDialog(QWidget *owner, QDialog *dlg, int w, int h)
{
    dlg->setParent(nullptr);
    dlg->setWindowModality(Qt::NonModal);
    dlg->setWindowFlags(Qt::Window |
                         Qt::WindowTitleHint |
                         Qt::WindowSystemMenuHint |
                         Qt::WindowMinimizeButtonHint |
                         Qt::WindowMaximizeButtonHint |
                         Qt::WindowCloseButtonHint);

    dlg->setAttribute(Qt::WA_DeleteOnClose, true);
    dlg->setSizeGripEnabled(true);

    QObject::connect(owner, &QObject::destroyed, dlg, &QWidget::close);

    const int maxW = int(owner->width()  * 0.85);
    const int maxH = int(owner->height() * 0.85);
    dlg->resize(qMin(w, maxW), qMin(h, maxH));

    const QPoint center = owner->frameGeometry().center();
    dlg->move(center.x() - dlg->width() / 2,
              center.y() - dlg->height() / 2);
}

} // namespace Dialogs
