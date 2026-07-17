#pragma once

#include <QWidget>
#include <functional>

namespace Dialogs {

void showFlapDialog(QWidget *owner, const std::function<void()> &redraw);

} // namespace Dialogs
