#include "ParsecDialog.h"
#include "ChildDialogFactory.h"
#include "../AirfoilGen/ParsecGenerator.h"
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

void showParsecDialog(QWidget *owner, const std::function<void()> &redraw)
{
    QDialog *dlg = new QDialog(owner);
    dlg->setWindowTitle("PARSEC Parameters");
    setupChildDialog(owner, dlg, 460, 460);

    QVBoxLayout *lay = new QVBoxLayout(dlg);
    auto addRow = [&](const QString &lbl, const QString &def) -> QLineEdit* {
        QHBoxLayout *h = new QHBoxLayout;
        h->addWidget(new QLabel(lbl));
        auto *e = new QLineEdit(def);
        h->addWidget(e);
        lay->addLayout(h);
        return e;
    };

    lay->addWidget(new QLabel("Leading edge:"));
    auto *rleE = addRow("rle:", "0.01");

    lay->addWidget(new QLabel("Upper surface crest:"));
    auto *xUpE   = addRow("xUp:", "0.30");
    auto *zUpE   = addRow("zUp:", "0.06");
    auto *zxxUpE = addRow("zxxUp:", "-0.40");

    lay->addWidget(new QLabel("Lower surface crest:"));
    auto *xLoE   = addRow("xLo:", "0.30");
    auto *zLoE   = addRow("zLo:", "-0.06");
    auto *zxxLoE = addRow("zxxLo:", "0.40");

    lay->addWidget(new QLabel("Trailing edge:"));
    auto *zteE      = addRow("zte:", "0.0");
    auto *deltaZteE = addRow("delta zte:", "0.0");
    auto *alphaTeE  = addRow("alpha TE (deg):", "0.0");
    auto *betaTeE   = addRow("beta TE (deg):", "10.0");

    QPushButton *gen = new QPushButton("Generate");
    lay->addWidget(gen);

    QObject::connect(gen, &QPushButton::clicked, dlg, [=]() {
        AirfoilGen::ParsecParams params;
        params.rle      = rleE->text().toDouble();
        params.xUp      = xUpE->text().toDouble();
        params.zUp      = zUpE->text().toDouble();
        params.zxxUp    = zxxUpE->text().toDouble();
        params.xLo      = xLoE->text().toDouble();
        params.zLo      = zLoE->text().toDouble();
        params.zxxLo    = zxxLoE->text().toDouble();
        params.zte      = zteE->text().toDouble();
        params.deltaZte = deltaZteE->text().toDouble();
        params.alphaTe  = alphaTeE->text().toDouble();
        params.betaTe   = betaTeE->text().toDouble();

        const AirfoilGen::ParsecResult result = AirfoilGen::generatePoints(params);
        if (!result.valid) {
            QMessageBox::warning(dlg, "Invalid PARSEC Parameters", result.errorMessage);
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
