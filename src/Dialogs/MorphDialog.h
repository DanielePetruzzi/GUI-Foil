#pragma once

#include <QWidget>
#include <functional>

namespace Dialogs {

// Interactive control-point morphing tool for the current airfoil.
void showMorphDialog(QWidget *owner, const std::function<void()> &redraw);

} // namespace Dialogs
