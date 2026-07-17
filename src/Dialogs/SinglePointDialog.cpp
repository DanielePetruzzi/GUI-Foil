#include "SinglePointDialog.h"
#include "ChildDialogFactory.h"
#include "../Plot/PlotColors.h"
#include "../Plot/PlotTransform.h"
#include "../Widgets/ResizableLabel.h"
#include "../Widgets/WindowUtils.h"
#include "../Utils/FileUtils.h"
#include "../XFoil/XFoilProcessManager.h"
#include "../XFoil/XFoilCommandBuilder.h"
#include "../XFoil/XFoilResultsReader.h"
#include "../XFoil/AirfoilFileRotation.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QFile>
#include <memory>
#include <cmath>

namespace Dialogs {

void showSinglePointDialog(QWidget *owner, QPointer<QDialog> *solutionDialog)
{
    if (!solutionDialog->isNull()) {
        WindowUtils::bringWindowToFront(solutionDialog->data());
        return;
    }

    QDialog *dlg = new QDialog(owner);
    *solutionDialog = dlg;
    QObject::connect(dlg, &QObject::destroyed, owner, [solutionDialog]() { *solutionDialog = nullptr; });

    dlg->setWindowTitle("Calculate Solution");
    setupChildDialog(owner, dlg, 960, 760);
    dlg->setMinimumSize(680, 520);

    QVBoxLayout *mainLay = new QVBoxLayout(dlg);

    // inputs row
    QHBoxLayout *inputs = new QHBoxLayout;
    auto addField = [&](const QString &lbl, const QString &def) -> QLineEdit* {
        inputs->addWidget(new QLabel(lbl));
        auto *e = new QLineEdit(def);
        e->setMaximumWidth(80);
        inputs->addWidget(e);
        return e;
    };
    auto *aoaEdit = addField("AoA (deg):", "0.0");

    QCheckBox *compressibleChk = new QCheckBox("Compressible");
    inputs->addWidget(compressibleChk);
    inputs->addWidget(new QLabel("Mach:"));
    auto *machEdit = new QLineEdit("0.0");
    machEdit->setMaximumWidth(80);
    machEdit->setEnabled(false);
    machEdit->setStyleSheet("QLineEdit:disabled { background-color: #E5E7EB; color: #6B7280; }");
    inputs->addWidget(machEdit);

    QCheckBox *viscousChk = new QCheckBox("Viscous");
    inputs->addWidget(viscousChk);
    inputs->addWidget(new QLabel("Reynolds:"));
    auto *reEdit = new QLineEdit("0");
    reEdit->setMaximumWidth(80);
    reEdit->setEnabled(false);
    reEdit->setStyleSheet("QLineEdit:disabled { background-color: #E5E7EB; color: #6B7280; }");
    inputs->addWidget(reEdit);
    inputs->addStretch();

    QPushButton *calcBtn = new QPushButton("Calculate");
    inputs->addWidget(calcBtn);
    mainLay->addLayout(inputs);

    QObject::connect(compressibleChk, &QCheckBox::toggled, machEdit, &QLineEdit::setEnabled);
    QObject::connect(viscousChk, &QCheckBox::toggled, reEdit, &QLineEdit::setEnabled);

    ResizableLabel *plotLbl = new ResizableLabel;
    plotLbl->setMinimumSize(600, 380);
    plotLbl->setStyleSheet("background-color: white; border: 1px solid #D1D5DB;");
    mainLay->addWidget(plotLbl, 1);

    QLabel *resultsLbl = new QLabel("AoA: \u2013   Ma: \u2013   Re: \u2013   CL: \u2013   CM: \u2013");
    resultsLbl->setAlignment(Qt::AlignCenter);
    resultsLbl->setStyleSheet("font-weight:bold;font-size:11px;margin:4px;");
    mainLay->addWidget(resultsLbl);

    auto curData  = std::make_shared<QVector<XFoil::CpPoint>>();
    auto prevData = std::make_shared<QVector<XFoil::CpPoint>>();
    auto curC     = std::make_shared<XFoil::Coefficients>();
    auto prevC    = std::make_shared<XFoil::Coefficients>();

    auto renderPlot = [=](int W, int H) {
        if (curData->isEmpty() || W <= 0 || H <= 0) return;

        const int mL = 60, mR = 40, mT = 40, mB = 50;
        double bMinY = 1e9, bMaxY = -1e9;

        auto expandY = [&](const QVector<XFoil::CpPoint> &d) {
            for (auto &p : d) {
                bMinY = qMin(bMinY, p.y);   bMaxY = qMax(bMaxY, p.y);
                bMinY = qMin(bMinY, -p.cp); bMaxY = qMax(bMaxY, -p.cp);
            }
        };
        expandY(*curData);
        if (!prevData->isEmpty()) expandY(*prevData);

        auto expandLift = [&](std::shared_ptr<XFoil::Coefficients> c) {
            if (!c->valid || c->cl == 0) return;
            double xcp = -(c->cm / c->cl);
            if (xcp > 0 && xcp < 1) {
                bMinY = qMin(bMinY, c->cl); bMaxY = qMax(bMaxY, c->cl);
                bMinY = qMin(bMinY, 0.0);   bMaxY = qMax(bMaxY, 0.0);
            }
        };
        expandLift(curC);
        expandLift(prevC);

        bMinY -= 0.1; bMaxY += 0.1;

        double pW = W - mL - mR, pH = H - mT - mB;
        double sX = pW / 1.0, sY = pH / (bMaxY - bMinY);
        double sc = qMin(sX, sY);
        double ox = mL + (pW - sc) / 2;
        double oy = mT + (pH - (bMaxY - bMinY) * sc) / 2;

        auto toS = [&](double x, double y) -> QPointF {
            return QPointF(ox + x * sc, oy + (bMaxY - y) * sc);
        };

        QPixmap px(W, H);
        px.fill(PlotColors::bg);
        QPainter pa(&px);
        pa.setRenderHint(QPainter::Antialiasing);

        // grid + x labels
        pa.setPen(QPen(PlotColors::grid, 0.5));
        for (double gx = 0; gx <= 1.05; gx += 0.2) {
            pa.drawLine(toS(gx, bMinY), toS(gx, bMaxY));
            pa.setPen(PlotColors::gridText);
            pa.setFont(QFont("Arial", 7));
            pa.drawText(int(toS(gx, bMinY).x()) - 8, H - mB + 14, QString::number(gx, 'f', 1));
            pa.setPen(QPen(PlotColors::grid, 0.5));
        }

        double stepY = 0.5;
        for (int i = int(std::floor(bMinY / stepY)); i <= int(std::ceil(bMaxY / stepY)); ++i) {
            double gy = i * stepY;
            pa.drawLine(toS(0, gy), toS(1, gy));
            pa.setPen(PlotColors::gridText);
            pa.setFont(QFont("Arial", 7));
            pa.drawText(mL - 36, int(toS(0, gy).y()) + 4, QString::number(gy, 'f', 1));
            pa.setPen(QPen(PlotColors::grid, 0.5));
        }

        pa.setPen(QPen(QColor("#9CA3AF"), 1, Qt::DashLine));
        pa.drawLine(toS(0, 0), toS(1, 0));

        pa.setPen(PlotColors::gridText);
        pa.setFont(QFont("Arial", 9));
        pa.drawText(int(ox + sc / 2) - 6, H - 10, "x/c");
        pa.save();
        pa.translate(14, H / 2);
        pa.rotate(-90);
        pa.drawText(-20, 0, "\u2013Cp / y");
        pa.restore();

        auto drawLift = [&](std::shared_ptr<XFoil::Coefficients> c, QColor col, Qt::PenStyle ps) {
            if (!c->valid || c->cl == 0) return;
            double xcp = -(c->cm / c->cl);
            if (xcp <= 0 || xcp >= 1) return;

            QPointF base = toS(xcp, 0), tip = toS(xcp, c->cl);
            pa.setPen(QPen(col, 1.8, ps));
            pa.setBrush(col);
            pa.drawLine(base, tip);

            double hs = 5.0;
            QPolygonF head;
            if (c->cl > 0)
                head << tip << QPointF(tip.x() - hs, tip.y() + hs * 1.5) << QPointF(tip.x() + hs, tip.y() + hs * 1.5);
            else
                head << tip << QPointF(tip.x() - hs, tip.y() - hs * 1.5) << QPointF(tip.x() + hs, tip.y() - hs * 1.5);
            pa.drawPolygon(head);
        };

        if (!prevData->isEmpty()) {
            QPolygonF fPoly, cPoly;
            for (auto &p : *prevData) { fPoly << toS(p.x, p.y); cPoly << toS(p.x, -p.cp); }
            pa.setPen(QPen(PlotColors::prevFoil, 1.2, Qt::DashLine)); pa.drawPolyline(fPoly);
            pa.setPen(QPen(PlotColors::prevCp, 1.2, Qt::DashLine));   pa.drawPolyline(cPoly);
            drawLift(prevC, PlotColors::prevLift, Qt::DashLine);
        }

        {
            QPolygonF fPoly, cPoly;
            for (auto &p : *curData) { fPoly << toS(p.x, p.y); cPoly << toS(p.x, -p.cp); }
            pa.setPen(QPen(PlotColors::curFoil, 1.8, Qt::SolidLine)); pa.drawPolyline(fPoly);
            pa.setPen(QPen(PlotColors::curCp, 2.0, Qt::SolidLine));   pa.drawPolyline(cPoly);
            drawLift(curC, PlotColors::curLift, Qt::SolidLine);
        }

        int lx = mL + 6, ly = mT + 6;
        PlotUtils::drawLegendEntry(pa, lx, ly, PlotColors::curFoil, Qt::SolidLine, "Airfoil (cur)", 20, 16);
        PlotUtils::drawLegendEntry(pa, lx, ly, PlotColors::curCp,   Qt::SolidLine, "\u2013Cp (cur)", 20, 16);
        if (!prevData->isEmpty()) {
            PlotUtils::drawLegendEntry(pa, lx, ly, PlotColors::prevFoil, Qt::DashLine, "Airfoil (prev)", 20, 16);
            PlotUtils::drawLegendEntry(pa, lx, ly, PlotColors::prevCp,   Qt::DashLine, "\u2013Cp (prev)", 20, 16);
        }

        plotLbl->setPixmap(px);
    };

    plotLbl->onResize = [=](int w, int h) { renderPlot(w, h); };

    QObject::connect(calcBtn, &QPushButton::clicked, dlg, [=]() {
        double aoa  = aoaEdit->text().toDouble();
        double mach = compressibleChk->isChecked() ? machEdit->text().toDouble() : 0.0;
        double re   = viscousChk->isChecked()      ? reEdit->text().toDouble()   : 0.0;

        const QString appDir  = AppPaths::appDir();
        const QString tempDir = AppPaths::tempDir();
        const QString inpPath = AppPaths::appFile("inp.dat");
        const QString clOut   = AppPaths::appFile("res.txt");
        const QString cpOut   = AppPaths::appFile("pres.txt");
        const QString resCur  = tempDir + "/Results.txt";
        const QString presCur = tempDir + "/Pressure.txt";
        const QString presPrev= tempDir + "/PressurePrev.txt";
        const QString instrF  = AppPaths::instructionFile();

        XFoil::rotateResultFiles(tempDir);

        QFile::remove(inpPath);
        QFile::copy(AppPaths::currentAirfoilDat(), inpPath);
        QFile::remove(clOut);
        QFile::remove(cpOut);

        const QString instr = XFoilCommands::singlePoint(aoa, mach, re, 500, "pres.txt", "res.txt");
        FileUtils::writeTextFile(instrF, instr);

        if (!XFoil::ProcessManager::instance().launch(appDir, instrF)) return;

        FileUtils::stripLinesAndMove(clOut, resCur, 12);
        FileUtils::stripLinesAndMove(cpOut, presCur, 2);

        if (curC->valid) *prevC = *curC;
        *curC = XFoil::readClCm(resCur);

        *prevData = XFoil::loadCpDistribution(presPrev);
        *curData  = XFoil::loadCpDistribution(presCur);

        resultsLbl->setText(QString("AoA: %1\u00b0   Ma: %2   Re: %3   CL: %4   CM: %5")
            .arg(aoaEdit->text())
            .arg(QString::number(mach, 'f', 4))
            .arg(QString::number(re, 'f', 0))
            .arg(curC->valid ? QString::number(curC->cl, 'f', 4) : "-")
            .arg(curC->valid ? QString::number(curC->cm, 'f', 4) : "-"));

        QFile::remove(inpPath);

        renderPlot(plotLbl->width(), plotLbl->height());
        WindowUtils::bringWindowToFront(dlg);
    });

    dlg->show();
}

} // namespace Dialogs
