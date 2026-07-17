#include "IgpDialog.h"
#include "ChildDialogFactory.h"
#include "../AirfoilGen/IgpGenerator.h"
#include "../XFoil/AirfoilFileRotation.h"
#include "../Utils/FileUtils.h"
#include "../Utils/DatFile.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

namespace Dialogs {

void showIgpDialog(QWidget *owner, const std::function<void()> &redraw)
{
    QDialog *dlg = new QDialog(owner);
    dlg->setWindowTitle("IGP Parameters");
    setupChildDialog(owner, dlg, 420, 350);

    QVBoxLayout *lay = new QVBoxLayout(dlg);
    auto addRow = [&](const QString &lbl, const QString &def) -> QLineEdit* {
        QHBoxLayout *h = new QHBoxLayout;
        h->addWidget(new QLabel(lbl));
        auto *e = new QLineEdit(def);
        h->addWidget(e);
        lay->addLayout(h);
        return e;
    };

    auto *c1E  = addRow("c1:", "0.30");
    auto *c2E  = addRow("c2:", "0.70");
    auto *c3E  = addRow("c3:", "0.05");
    auto *c4E  = addRow("c4:", "0.00");
    auto *tE   = addRow("T:", "0.12");
    auto *xtE  = addRow("XT:", "0.30");
    auto *rhoE = addRow("rho0:", "1.00");
    auto *betE = addRow("betaTE:", "1.00");

    QPushButton *gen = new QPushButton("Generate");
    lay->addWidget(gen);

    QObject::connect(gen, &QPushButton::clicked, dlg, [=]() {
        AirfoilGen::IgpParams params;
        params.c1 = c1E->text().toDouble();
        params.c2 = c2E->text().toDouble();
        params.c3 = c3E->text().toDouble();
        params.c4 = c4E->text().toDouble();
        params.T  = tE->text().toDouble();
        params.XT = xtE->text().toDouble();
        params.rho0Input = rhoE->text().toDouble();
        params.betaInput = betE->text().toDouble();

        const AirfoilGen::IgpResult result = AirfoilGen::generatePoints(params);
        if (!result.valid) {
            QMessageBox::warning(dlg, "Invalid IGP Parameters", result.errorMessage);
            return;
        }

        const QString header = AirfoilGen::buildHeader(params);
        const QString tempDir = AppPaths::tempDir();
        XFoil::rotateCurrentToPrevious(tempDir);
        DatFile::save(AppPaths::currentAirfoilDat(), header, result.points);

        redraw();
        dlg->accept();
    });

    dlg->show();
}

} // namespace Dialogs
