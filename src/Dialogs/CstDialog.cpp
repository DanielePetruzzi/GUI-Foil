#include "CstDialog.h"
#include "ChildDialogFactory.h"
#include "../AirfoilGen/CstGenerator.h"
#include "../XFoil/AirfoilFileRotation.h"
#include "../Utils/FileUtils.h"
#include "../Utils/DatFile.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace Dialogs {

void showCstDialog(QWidget *owner, const std::function<void()> &redraw)
{
    QDialog *dlg = new QDialog(owner);
    dlg->setWindowTitle("CST Parameters");
    setupChildDialog(owner, dlg, 420, 340);

    QVBoxLayout *lay = new QVBoxLayout(dlg);
    auto addRow = [&](const QString &lbl, const QString &def = "0.0") -> QLineEdit* {
        QHBoxLayout *h = new QHBoxLayout;
        h->addWidget(new QLabel(lbl));
        auto *e = new QLineEdit(def); h->addWidget(e);
        lay->addLayout(h); return e;
    };
    auto *n1E = addRow("N1:"); auto *n2E = addRow("N2:");

    lay->addWidget(new QLabel("Upper coefficients (A1U … A4U):"));
    QHBoxLayout *uL = new QHBoxLayout;
    auto *a1uE = new QLineEdit("0.0"); auto *a2uE = new QLineEdit("0.0");
    auto *a3uE = new QLineEdit("0.0"); auto *a4uE = new QLineEdit("0.0");
    uL->addWidget(a1uE); uL->addWidget(a2uE); uL->addWidget(a3uE); uL->addWidget(a4uE);
    lay->addLayout(uL);

    lay->addWidget(new QLabel("Lower coefficients (A1L … A4L):"));
    QHBoxLayout *lL = new QHBoxLayout;
    auto *a1lE = new QLineEdit("0.0"); auto *a2lE = new QLineEdit("0.0");
    auto *a3lE = new QLineEdit("0.0"); auto *a4lE = new QLineEdit("0.0");
    lL->addWidget(a1lE); lL->addWidget(a2lE); lL->addWidget(a3lE); lL->addWidget(a4lE);
    lay->addLayout(lL);

    QPushButton *gen = new QPushButton("Generate");
    lay->addWidget(gen);

    QObject::connect(gen, &QPushButton::clicked, dlg, [=]() {
        AirfoilGen::CstParams params;
        params.n1 = n1E->text().toDouble();
        params.n2 = n2E->text().toDouble();
        params.upper[0] = a1uE->text().toDouble();
        params.upper[1] = a2uE->text().toDouble();
        params.upper[2] = a3uE->text().toDouble();
        params.upper[3] = a4uE->text().toDouble();
        params.lower[0] = a1lE->text().toDouble();
        params.lower[1] = a2lE->text().toDouble();
        params.lower[2] = a3lE->text().toDouble();
        params.lower[3] = a4lE->text().toDouble();

        const QVector<QPointF> pts = AirfoilGen::generatePoints(params);
        const QString header = AirfoilGen::buildHeader(params);

        const QString tempDir = AppPaths::tempDir();
        XFoil::rotateCurrentToPrevious(tempDir);
        DatFile::save(AppPaths::currentAirfoilDat(), header, pts);

        redraw();
        dlg->accept();
    });

    dlg->show();
}

} // namespace Dialogs
