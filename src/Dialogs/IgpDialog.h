#pragma once

#include <QWidget>
#include <functional>

namespace Dialogs {

void showIgpDialog(QWidget *owner, const std::function<void()> &redraw);

} // namespace Dialogs
