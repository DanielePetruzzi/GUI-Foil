#include "SolutionChoiceDialog.h"
#include "ChildDialogFactory.h"
#include "SinglePointDialog.h"
#include "PolarDialog.h"
#include "../Widgets/WindowUtils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace Dialogs {

void showSolutionChoiceDialog(QWidget *owner, QPointer<QDialog> *solutionDialog)
{
    if (!solutionDialog->isNull()) {
        WindowUtils::bringWindowToFront(solutionDialog->data());
        return;
    }

    QDialog *dlg = new QDialog(owner);
    *solutionDialog = dlg;
    QObject::connect(dlg, &QObject::destroyed, owner, [solutionDialog]() { *solutionDialog = nullptr; });

    dlg->setWindowTitle("Calculate Solution");
    setupChildDialog(owner, dlg, 420, 180);
    dlg->setMinimumSize(360, 150);

    QVBoxLayout *mainLay = new QVBoxLayout(dlg);

    QLabel *titleLbl = new QLabel("Select analysis type");
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setStyleSheet("font-weight:bold;font-size:14px;margin:8px;");
    mainLay->addWidget(titleLbl);

    QLabel *infoLbl = new QLabel("Choose between a single operating point or a viscous polar sweep.");
    infoLbl->setAlignment(Qt::AlignCenter);
    infoLbl->setStyleSheet("color:#4B5563;margin-bottom:8px;");
    mainLay->addWidget(infoLbl);

    QHBoxLayout *btnLay = new QHBoxLayout;

    QPushButton *singleBtn = new QPushButton("Single Point");
    QPushButton *polarBtn  = new QPushButton("Polar");
    singleBtn->setMinimumHeight(34);
    polarBtn->setMinimumHeight(34);

    btnLay->addStretch();
    btnLay->addWidget(singleBtn);
    btnLay->addSpacing(12);
    btnLay->addWidget(polarBtn);
    btnLay->addStretch();

    mainLay->addLayout(btnLay);

    QObject::connect(singleBtn, &QPushButton::clicked, dlg, [=]() {
        *solutionDialog = nullptr;
        dlg->hide();
        dlg->deleteLater();
        showSinglePointDialog(owner, solutionDialog);
    });

    QObject::connect(polarBtn, &QPushButton::clicked, dlg, [=]() {
        *solutionDialog = nullptr;
        dlg->hide();
        dlg->deleteLater();
        showPolarDialog(owner, solutionDialog);
    });

    dlg->show();
}

} // namespace Dialogs
