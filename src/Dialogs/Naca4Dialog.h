#pragma once

#include <QWidget>
#include <functional>

namespace Dialogs {

void showNaca4Dialog(QWidget *owner, const std::function<void()> &redraw);

} // namespace Dialogs
