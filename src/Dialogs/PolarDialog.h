#pragma once

#include <QWidget>
#include <QDialog>
#include <QPointer>

namespace Dialogs {

void showPolarDialog(QWidget *owner, QPointer<QDialog> *solutionDialog);

} // namespace Dialogs
