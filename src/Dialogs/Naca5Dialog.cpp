#include "Naca5Dialog.h"
#include "ChildDialogFactory.h"
#include "../AirfoilGen/Naca5Generator.h"
#include "../AirfoilGen/AirfoilParams.h"
#include "../XFoil/XFoilProcessManager.h"
#include "../XFoil/AirfoilFileRotation.h"
#include "../Utils/FileUtils.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace Dialogs {

void showNaca5Dialog(QWidget *owner, const std::function<void()> &redraw)
{
    QDialog *dlg = new QDialog(owner);
    dlg->setWindowTitle("NACA 5 Digits");
    setupChildDialog(owner, dlg, 300, 230);

    QVBoxLayout *lay = new QVBoxLayout(dlg);
    auto addRow = [&](const QString &lbl) -> QLineEdit* {
        lay->addWidget(new QLabel(lbl));
        auto *e = new QLineEdit; lay->addWidget(e); return e;
    };
    auto *lEdit  = addRow("L:");
    auto *p5Edit = addRow("P:");
    auto *qEdit  = addRow("Q:");
    auto *xxEdit = addRow("XX:");

    QPushButton *gen = new QPushButton("Generate");
    lay->addWidget(gen);

    QObject::connect(gen, &QPushButton::clicked, dlg, [=]() {
        L  = lEdit->text().toInt();
        P5 = p5Edit->text().toInt();
        Q  = qEdit->text().toInt();
        XX = xxEdit->text().toInt();

        const QString instr = AirfoilGen::buildInstruction({ L, P5, Q, XX });

        XFoil::rotateCurrentToPrevious(AppPaths::tempDir());
        XFoil::runXfoil(instr);
        redraw();
        dlg->accept();
    });

    dlg->show();
}

} // namespace Dialogs
