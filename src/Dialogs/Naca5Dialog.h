#pragma once

#include <QWidget>
#include <functional>

namespace Dialogs {

void showNaca5Dialog(QWidget *owner, const std::function<void()> &redraw);

} // namespace Dialogs
