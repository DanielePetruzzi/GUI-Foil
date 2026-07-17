#include "Naca4Dialog.h"
#include "ChildDialogFactory.h"
#include "../AirfoilGen/Naca4Generator.h"
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

void showNaca4Dialog(QWidget *owner, const std::function<void()> &redraw)
{
    QDialog *dlg = new QDialog(owner);
    dlg->setWindowTitle("NACA 4 Digits");
    setupChildDialog(owner, dlg, 300, 200);

    QVBoxLayout *lay = new QVBoxLayout(dlg);
    auto addRow = [&](const QString &lbl) -> QLineEdit* {
        lay->addWidget(new QLabel(lbl));
        auto *e = new QLineEdit; lay->addWidget(e); return e;
    };
    auto *mEdit  = addRow("M (max camber %):");
    auto *p4Edit = addRow("P (camber position ×10):");
    auto *sEdit  = addRow("SS (thickness %):");

    QPushButton *gen = new QPushButton("Generate");
    lay->addWidget(gen);

    QObject::connect(gen, &QPushButton::clicked, dlg, [=]() {
        M  = mEdit->text().toInt();
        P4 = p4Edit->text().toInt();
        SS = sEdit->text().toInt();

        const QString instr = AirfoilGen::buildInstruction({ M, P4, SS });

        XFoil::rotateCurrentToPrevious(AppPaths::tempDir());
        XFoil::runXfoil(instr);
        redraw();
        dlg->accept();
    });

    dlg->show();
}

} // namespace Dialogs
