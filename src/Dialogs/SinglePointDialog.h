#pragma once

#include <QWidget>
#include <QDialog>
#include <QPointer>

namespace Dialogs {

void showSinglePointDialog(QWidget *owner, QPointer<QDialog> *solutionDialog);

} // namespace Dialogs
