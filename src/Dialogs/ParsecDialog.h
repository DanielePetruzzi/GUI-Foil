#pragma once

#include <QWidget>
#include <functional>

namespace Dialogs {

void showParsecDialog(QWidget *owner, const std::function<void()> &redraw);

} // namespace Dialogs
