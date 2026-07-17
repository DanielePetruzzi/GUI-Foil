#pragma once

#include <QWidget>
#include <functional>

namespace Dialogs {

void showRepanelDialog(QWidget *owner, const std::function<void()> &redraw);

} // namespace Dialogs
