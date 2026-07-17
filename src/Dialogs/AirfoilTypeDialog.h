#pragma once

#include <QWidget>
#include <functional>

namespace Dialogs {

// Small chooser dialog that opens one of the four airfoil generator
// dialogs. redraw is forwarded to whichever generator dialog is opened,
// so the main plot refreshes once a new airfoil is produced.
void showAirfoilTypeDialog(QWidget *owner, const std::function<void()> &redraw);

} // namespace Dialogs
