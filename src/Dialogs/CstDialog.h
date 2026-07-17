#pragma once

#include <QWidget>
#include <functional>

namespace Dialogs {

void showCstDialog(QWidget *owner, const std::function<void()> &redraw);

} // namespace Dialogs
