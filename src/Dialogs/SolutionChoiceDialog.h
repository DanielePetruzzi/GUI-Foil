#pragma once

#include <QWidget>
#include <QDialog>
#include <QPointer>
#include <functional>

namespace Dialogs {

// Small chooser dialog opened by "Calculate Solution": lets the user pick
// between a single operating point or a full polar sweep. solutionDialog
// is the shared singleton pointer (owned by MainWindow) that tracks
// whichever solution-plot dialog is currently open, so opening a new one
// closes/reuses the previous one instead of stacking windows.
void showSolutionChoiceDialog(QWidget *owner, QPointer<QDialog> *solutionDialog);

} // namespace Dialogs
