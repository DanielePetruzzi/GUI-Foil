#pragma once

#include <QDialog>
#include <QWidget>

namespace Dialogs {

// Configures dlg as a non-modal, freely movable child window centered on
// owner, capped at 85% of owner's size, and closed automatically when
// owner is destroyed. Every dialog in the app is set up this way.
void setupChildDialog(QWidget *owner, QDialog *dlg, int w, int h);

} // namespace Dialogs
