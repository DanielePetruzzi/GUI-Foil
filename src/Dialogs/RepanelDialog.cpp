#include "RepanelDialog.h"
#include "ChildDialogFactory.h"
#include "../XFoil/XFoilProcessManager.h"
#include "../XFoil/XFoilCommandBuilder.h"
#include "../Utils/FileUtils.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>

namespace Dialogs {

void showRepanelDialog(QWidget *owner, const std::function<void()> &redraw)
{
    QDialog *dlg = new QDialog(owner);
    dlg->setWindowTitle("Repanel Airfoil");
    setupChildDialog(owner, dlg, 280, 140);

    QVBoxLayout *lay = new QVBoxLayout(dlg);
    lay->addWidget(new QLabel("Number of panels:"));
    QLineEdit *npEdit = new QLineEdit;
    lay->addWidget(npEdit);
    QPushButton *ok = new QPushButton("Apply");
    lay->addWidget(ok);

    QObject::connect(ok, &QPushButton::clicked, dlg, [=]() {
        const int NP = npEdit->text().toInt();
        if (NP <= 0)
            return;

        const QString appDir   = AppPaths::appDir();
        const QString tempDir  = AppPaths::tempDir();
        const QString curPath  = AppPaths::currentAirfoilDat();
        const QString prevPath = AppPaths::previousAirfoilDat();
        const QString tmpPath  = AppPaths::appFile("tmprep.dat");
        const QString outPath  = AppPaths::appFile("cur.dat");
        const QString instrFile = AppPaths::instructionFile();

        QFile::remove(tmpPath);
        QFile::copy(curPath, tmpPath);
        QFile::remove(prevPath);
        QFile::rename(curPath, prevPath);

        const QString instr = XFoilCommands::repanel(tmpPath, NP, outPath);
        FileUtils::writeTextFile(instrFile, instr);

        if (!XFoil::ProcessManager::instance().launch(appDir, instrFile))
            return;

        if (QFile::exists(outPath)) {
            QFile::remove(curPath);
            QFile::rename(outPath, curPath);
        }
        QFile::remove(tmpPath);

        redraw();
        dlg->accept();
    });

    dlg->show();
}

} // namespace Dialogs
