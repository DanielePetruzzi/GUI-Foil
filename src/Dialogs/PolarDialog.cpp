#include "PolarDialog.h"
#include "ChildDialogFactory.h"
#include "../Plot/PlotColors.h"
#include "../Widgets/ResizableLabel.h"
#include "../Widgets/WindowUtils.h"
#include "../Utils/FileUtils.h"
#include "../XFoil/XFoilProcessManager.h"
#include "../XFoil/XFoilCommandBuilder.h"
#include "../XFoil/XFoilResultsReader.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QLocale>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <memory>
#include <cmath>
#include <limits>

namespace Dialogs {

void showPolarDialog(QWidget *owner, QPointer<QDialog> *solutionDialog)
{
    if (!solutionDialog->isNull()) {
        WindowUtils::bringWindowToFront(solutionDialog->data());
        return;
    }

    QDialog *dlg = new QDialog(owner);
    *solutionDialog = dlg;
    QObject::connect(dlg, &QObject::destroyed, owner, [solutionDialog]() { *solutionDialog = nullptr; });

    dlg->setWindowTitle("Calculate Polar");
    setupChildDialog(owner, dlg, 1680, 760);
    dlg->setMinimumSize(1230, 560);

    QVBoxLayout *mainLay = new QVBoxLayout(dlg);

    // ---------------------------------------------------------- inputs
    QHBoxLayout *inputs = new QHBoxLayout;
    auto addEdit = [=](const QString &lbl, const QString &def, int w) -> QLineEdit* {
        inputs->addWidget(new QLabel(lbl));
        auto *e = new QLineEdit(def);
        e->setMaximumWidth(w);
        inputs->addWidget(e);
        return e;
    };
    auto *aoaStartEdit = addEdit("AoA start:", "-4.0", 80);
    auto *aoaEndEdit   = addEdit("AoA end:",   "12.0", 80);
    auto *aoaStepEdit  = addEdit("Step:",      "0.5",  70);
    auto *reEdit       = addEdit("Reynolds:",  "1000000", 95);

    QCheckBox *compressibleChk = new QCheckBox("Compressible");
    inputs->addWidget(compressibleChk);
    inputs->addWidget(new QLabel("Mach:"));
    auto *machEdit = new QLineEdit("0.0");
    machEdit->setMaximumWidth(80);
    machEdit->setEnabled(false);
    machEdit->setStyleSheet("QLineEdit:disabled { background-color: #E5E7EB; color: #6B7280; }");
    inputs->addWidget(machEdit);

    auto *iterEdit = addEdit("Iterations:", "500", 70);

    QLabel *nStepsLbl = new QLabel("Points: \u2013");
    nStepsLbl->setMinimumWidth(80);
    nStepsLbl->setAlignment(Qt::AlignCenter);
    nStepsLbl->setStyleSheet("font-weight:bold;color:#6B7280;");
    inputs->addWidget(nStepsLbl);

    QPushButton *calcBtn = new QPushButton("Calculate");
    inputs->addWidget(calcBtn);
    QPushButton *exportBtn = new QPushButton("Export Polar CSV");
    exportBtn->setEnabled(false);
    inputs->addWidget(exportBtn);
    QPushButton *clearBtn = new QPushButton("Clear Polar");
    clearBtn->setEnabled(false);
    inputs->addWidget(clearBtn);

    mainLay->addLayout(inputs);

    QObject::connect(compressibleChk, &QCheckBox::toggled, machEdit, &QLineEdit::setEnabled);

    // ------------------------------------------------------- validators
    QLocale numericLocale = QLocale::C;
    auto makeDoubleValidator = [=](double bottom, double top, int decimals, QObject *parent) -> QDoubleValidator* {
        auto *v = new QDoubleValidator(bottom, top, decimals, parent);
        v->setNotation(QDoubleValidator::StandardNotation);
        v->setLocale(numericLocale);
        return v;
    };
    aoaStartEdit->setValidator(makeDoubleValidator(-90.0, 90.0, 4, aoaStartEdit));
    aoaEndEdit->setValidator(makeDoubleValidator(-90.0, 90.0, 4, aoaEndEdit));
    aoaStepEdit->setValidator(makeDoubleValidator(0.000001, 90.0, 6, aoaStepEdit));
    reEdit->setValidator(makeDoubleValidator(1.0, 1.0e12, 2, reEdit));
    machEdit->setValidator(makeDoubleValidator(0.0, 5.0, 6, machEdit));
    iterEdit->setValidator(new QIntValidator(1, 100000, iterEdit));

    auto parseDoubleEdit = [](QLineEdit *edit, bool *ok) -> double {
        QString s = edit->text().trimmed();
        s.replace(',', '.');
        return s.toDouble(ok);
    };

    // ------------------------------------------------------- plot area
    QGridLayout *plotGrid = new QGridLayout;
    ResizableLabel *polarLbl   = new ResizableLabel;
    ResizableLabel *clAlphaLbl = new ResizableLabel;
    ResizableLabel *cdAlphaLbl = new ResizableLabel;

    auto initPlotLabel = [](ResizableLabel *lbl) {
        lbl->setMinimumSize(300, 220);
        lbl->setStyleSheet("background-color: white; border: 1px solid #D1D5DB;");
    };
    initPlotLabel(polarLbl);
    initPlotLabel(clAlphaLbl);
    initPlotLabel(cdAlphaLbl);

    plotGrid->addWidget(polarLbl,   0, 0, 2, 1);
    plotGrid->addWidget(clAlphaLbl, 0, 1, 1, 1);
    plotGrid->addWidget(cdAlphaLbl, 1, 1, 1, 1);
    plotGrid->setColumnStretch(0, 2);
    plotGrid->setColumnStretch(1, 1);
    plotGrid->setRowStretch(0, 1);
    plotGrid->setRowStretch(1, 1);
    mainLay->addLayout(plotGrid, 1);

    QLabel *summaryLbl = new QLabel("Points: \u2013   CLmax: \u2013   CDmin: \u2013   Max CL/CD: \u2013");
    summaryLbl->setAlignment(Qt::AlignCenter);
    summaryLbl->setStyleSheet("font-weight:bold;font-size:11px;margin:4px;");
    mainLay->addWidget(summaryLbl);

    auto polarData = std::make_shared<QVector<XFoil::PolarPoint>>();

    // -------------------------------------------------- point counter
    auto computeRequestedPoints = [=]() -> int {
        bool okA0 = false, okA1 = false, okS = false;
        double a0 = parseDoubleEdit(aoaStartEdit, &okA0);
        double a1 = parseDoubleEdit(aoaEndEdit, &okA1);
        double st = parseDoubleEdit(aoaStepEdit, &okS);
        if (!okA0 || !okA1 || !okS) return -1;
        if (a0 >= a1) return -1;
        if (st <= 0.0) return -1;
        return int(std::floor((a1 - a0) / st + 1e-9)) + 1;
    };

    auto updateStepCount = [=]() {
        int n = computeRequestedPoints();
        if (n <= 0) {
            nStepsLbl->setText("Points: \u2013");
            nStepsLbl->setStyleSheet("font-weight:bold;color:#6B7280;");
        } else if (n > 200) {
            nStepsLbl->setText(QString("Points: %1").arg(n));
            nStepsLbl->setStyleSheet("font-weight:bold;color:#DC2626;");
        } else {
            nStepsLbl->setText(QString("Points: %1").arg(n));
            nStepsLbl->setStyleSheet("font-weight:bold;color:#047857;");
        }
    };

    QObject::connect(aoaStartEdit, &QLineEdit::textChanged, dlg, [=](const QString &) { updateStepCount(); });
    QObject::connect(aoaEndEdit,   &QLineEdit::textChanged, dlg, [=](const QString &) { updateStepCount(); });
    QObject::connect(aoaStepEdit,  &QLineEdit::textChanged, dlg, [=](const QString &) { updateStepCount(); });
    updateStepCount();

    // -------------------------------------------- generic XY plot renderer
    auto renderXYPlot = [=](ResizableLabel *lbl, const QVector<QPointF> &pts,
                             const QString &title, const QString &xLabel, const QString &yLabel,
                             int W, int H) {
        if (W <= 0 || H <= 0) return;

        QPixmap px(W, H);
        px.fill(PlotColors::bg);
        QPainter pa(&px);
        pa.setRenderHint(QPainter::Antialiasing);

        const int mL = 58, mR = 24, mT = 36, mB = 46;

        pa.setPen(PlotColors::gridText);
        pa.setFont(QFont("Arial", 10, QFont::Bold));
        pa.drawText(QRect(0, 6, W, 22), Qt::AlignCenter, title);

        if (pts.isEmpty()) {
            pa.setFont(QFont("Arial", 9));
            pa.setPen(QColor("#6B7280"));
            pa.drawText(QRect(0, 0, W, H), Qt::AlignCenter, "No data");
            lbl->setPixmap(px);
            return;
        }

        double xmin = std::numeric_limits<double>::max(), xmax = -std::numeric_limits<double>::max();
        double ymin = std::numeric_limits<double>::max(), ymax = -std::numeric_limits<double>::max();
        for (const QPointF &p : pts) {
            xmin = qMin(xmin, p.x()); xmax = qMax(xmax, p.x());
            ymin = qMin(ymin, p.y()); ymax = qMax(ymax, p.y());
        }
        if (std::abs(xmax - xmin) < 1e-12) { xmin -= 0.5; xmax += 0.5; }
        if (std::abs(ymax - ymin) < 1e-12) { ymin -= 0.5; ymax += 0.5; }

        double dx = xmax - xmin, dy = ymax - ymin;
        xmin -= 0.08 * dx; xmax += 0.08 * dx;
        ymin -= 0.12 * dy; ymax += 0.12 * dy;

        double pW = W - mL - mR, pH = H - mT - mB;
        auto toS = [=](double x, double y) -> QPointF {
            double sx = mL + (x - xmin) / (xmax - xmin) * pW;
            double sy = mT + (ymax - y) / (ymax - ymin) * pH;
            return QPointF(sx, sy);
        };

        QRectF plotRect(mL, mT, pW, pH);
        pa.setPen(QPen(QColor("#D1D5DB"), 1));
        pa.setBrush(Qt::NoBrush);
        pa.drawRect(plotRect);

        const int nGrid = 5;
        pa.setFont(QFont("Arial", 7));
        for (int i = 0; i <= nGrid; ++i) {
            double tx = xmin + (xmax - xmin) * double(i) / double(nGrid);
            QPointF a = toS(tx, ymin), b = toS(tx, ymax);
            pa.setPen(QPen(PlotColors::grid, 0.5)); pa.drawLine(a, b);
            pa.setPen(PlotColors::gridText);
            pa.drawText(int(a.x()) - 18, H - mB + 15, QString::number(tx, 'g', 4));
        }
        for (int i = 0; i <= nGrid; ++i) {
            double ty = ymin + (ymax - ymin) * double(i) / double(nGrid);
            QPointF a = toS(xmin, ty), b = toS(xmax, ty);
            pa.setPen(QPen(PlotColors::grid, 0.5)); pa.drawLine(a, b);
            pa.setPen(PlotColors::gridText);
            pa.drawText(6, int(a.y()) + 4, QString::number(ty, 'g', 4));
        }

        pa.setPen(QPen(QColor("#9CA3AF"), 1, Qt::DashLine));
        if (xmin < 0.0 && xmax > 0.0) pa.drawLine(toS(0.0, ymin), toS(0.0, ymax));
        if (ymin < 0.0 && ymax > 0.0) pa.drawLine(toS(xmin, 0.0), toS(xmax, 0.0));

        QPolygonF poly;
        for (const QPointF &p : pts) poly << toS(p.x(), p.y());
        pa.setPen(QPen(PlotColors::curCp, 2.0, Qt::SolidLine));
        pa.drawPolyline(poly);

        pa.setBrush(PlotColors::curCp);
        pa.setPen(Qt::NoPen);
        for (const QPointF &p : pts) pa.drawEllipse(toS(p.x(), p.y()), 2.5, 2.5);

        pa.setPen(PlotColors::gridText);
        pa.setFont(QFont("Arial", 9));
        pa.drawText(QRect(mL, H - 24, int(pW), 18), Qt::AlignCenter, xLabel);
        pa.save();
        pa.translate(14, mT + pH / 2.0);
        pa.rotate(-90);
        pa.drawText(QRect(-80, -8, 160, 18), Qt::AlignCenter, yLabel);
        pa.restore();

        lbl->setPixmap(px);
    };

    auto renderAllPlots = [=]() {
        QVector<QPointF> clcdPts, clAlphaPts, cdAlphaPts;
        for (const XFoil::PolarPoint &p : *polarData) {
            clcdPts.append(QPointF(p.cd, p.cl));
            clAlphaPts.append(QPointF(p.alpha, p.cl));
            cdAlphaPts.append(QPointF(p.alpha, p.cd));
        }
        renderXYPlot(polarLbl,   clcdPts,    "Polar: CL vs CD", "CD",          "CL", polarLbl->width(),   polarLbl->height());
        renderXYPlot(clAlphaLbl, clAlphaPts, "CL vs Alpha",     "Alpha [deg]", "CL", clAlphaLbl->width(), clAlphaLbl->height());
        renderXYPlot(cdAlphaLbl, cdAlphaPts, "CD vs Alpha",     "Alpha [deg]", "CD", cdAlphaLbl->width(), cdAlphaLbl->height());
    };

    polarLbl->onResize   = [=](int, int) { renderAllPlots(); };
    clAlphaLbl->onResize = [=](int, int) { renderAllPlots(); };
    cdAlphaLbl->onResize = [=](int, int) { renderAllPlots(); };

    auto updateSummary = [=]() {
        if (polarData->isEmpty()) {
            summaryLbl->setText("Points: \u2013   CLmax: \u2013   CDmin: \u2013   Max CL/CD: \u2013");
            return;
        }
        double clMax = -std::numeric_limits<double>::max(), clMaxAlpha = 0.0;
        double cdMin = std::numeric_limits<double>::max(),  cdMinAlpha = 0.0;
        double ldMax = -std::numeric_limits<double>::max(), ldMaxAlpha = 0.0;
        for (const XFoil::PolarPoint &p : *polarData) {
            if (p.cl > clMax) { clMax = p.cl; clMaxAlpha = p.alpha; }
            if (p.cd > 0.0 && p.cd < cdMin) { cdMin = p.cd; cdMinAlpha = p.alpha; }
            if (p.cd > 0.0) { double ld = p.cl / p.cd; if (ld > ldMax) { ldMax = ld; ldMaxAlpha = p.alpha; } }
        }
        summaryLbl->setText(QString("Points: %1   CLmax: %2 @ %3\u00b0   CDmin: %4 @ %5\u00b0   Max CL/CD: %6 @ %7\u00b0")
            .arg(polarData->size())
            .arg(clMax, 0, 'f', 4).arg(clMaxAlpha, 0, 'f', 2)
            .arg(cdMin, 0, 'f', 5).arg(cdMinAlpha, 0, 'f', 2)
            .arg(ldMax, 0, 'f', 2).arg(ldMaxAlpha, 0, 'f', 2));
    };

    renderAllPlots();

    QObject::connect(clearBtn, &QPushButton::clicked, dlg, [=]() {
        polarData->clear();
        exportBtn->setEnabled(false);
        clearBtn->setEnabled(false);
        summaryLbl->setText("Points: \u2013   CLmax: \u2013   CDmin: \u2013   Max CL/CD: \u2013");
        renderAllPlots();
    });

    QObject::connect(exportBtn, &QPushButton::clicked, dlg, [=]() {
        if (polarData->isEmpty()) return;
        QString fileName = QFileDialog::getSaveFileName(dlg, "Export Polar CSV",
            QDir::homePath() + "/polar.csv", "CSV files (*.csv)");
        if (fileName.isEmpty()) return;

        QFile f(fileName);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(dlg, "Export Error", "Unable to write CSV file.");
            return;
        }
        QTextStream out(&f);
        out << "alpha,CL,CD,CDp,CM,Top_Xtr,Bot_Xtr,CL_CD\n";
        for (const XFoil::PolarPoint &p : *polarData) {
            double ld = p.cd > 0.0 ? p.cl / p.cd : 0.0;
            out << QString::number(p.alpha,  'f', 6) << ","
                << QString::number(p.cl,     'f', 8) << ","
                << QString::number(p.cd,     'f', 8) << ","
                << QString::number(p.cdp,    'f', 8) << ","
                << QString::number(p.cm,     'f', 8) << ","
                << QString::number(p.topXtr, 'f', 8) << ","
                << QString::number(p.botXtr, 'f', 8) << ","
                << QString::number(ld,       'f', 8) << "\n";
        }
    });

    QObject::connect(calcBtn, &QPushButton::clicked, dlg, [=]() {
        bool okA0=false, okA1=false, okS=false, okRe=false, okMach=false, okIter=false;
        double aoaStart = parseDoubleEdit(aoaStartEdit, &okA0);
        double aoaEnd   = parseDoubleEdit(aoaEndEdit, &okA1);
        double aoaStep  = parseDoubleEdit(aoaStepEdit, &okS);
        double re       = parseDoubleEdit(reEdit, &okRe);
        double mach     = compressibleChk->isChecked() ? parseDoubleEdit(machEdit, &okMach) : 0.0;
        int iter        = iterEdit->text().toInt(&okIter);
        if (!compressibleChk->isChecked()) okMach = true;

        if (!okA0 || !okA1 || !okS || !okRe || !okMach || !okIter) {
            QMessageBox::warning(dlg, "Invalid Input", "Please check all numeric input fields.");
            return;
        }
        if (aoaStart >= aoaEnd) {
            QMessageBox::warning(dlg, "Invalid AoA Range", "AoA start must be smaller than AoA end.");
            return;
        }
        if (aoaStep <= 0.0) {
            QMessageBox::warning(dlg, "Invalid Step", "AoA step must be greater than zero.");
            return;
        }
        if (re <= 0.0) {
            QMessageBox::warning(dlg, "Invalid Reynolds", "Reynolds must be greater than zero for polar analysis.");
            return;
        }
        if (iter <= 0) {
            QMessageBox::warning(dlg, "Invalid Iterations", "Iterations must be greater than zero.");
            return;
        }

        int nReq = int(std::floor((aoaEnd - aoaStart) / aoaStep + 1e-9)) + 1;
        if (nReq > 200) {
            QMessageBox::warning(dlg, "Too Many Points",
                QString("Requested polar points: %1.\nPlease reduce the AoA range or increase the step.").arg(nReq));
            return;
        }

        const QString appDir  = AppPaths::appDir();
        const QString tempDir = AppPaths::tempDir();
        QDir().mkpath(tempDir);

        const QString inpPath     = AppPaths::appFile("inp.dat");
        const QString polarOut    = AppPaths::appFile("polar.txt");
        const QString polarCur    = tempDir + "/Polar.txt";
        const QString polarInstr  = tempDir + "/xfoil_polar_instr.txt";
        const QString currentFoil = AppPaths::currentAirfoilDat();

        if (!QFile::exists(currentFoil)) {
            QMessageBox::critical(dlg, "Missing Airfoil", "CurrentAirfoil.dat was not found in the temp folder.");
            return;
        }

        QFile::remove(inpPath);
        if (!QFile::copy(currentFoil, inpPath)) {
            QMessageBox::critical(dlg, "File Error", "Unable to copy CurrentAirfoil.dat to inp.dat.");
            return;
        }

        QFile::remove(polarOut);
        QFile::remove(polarCur);

        const QString instr = XFoilCommands::polarSweep(re, mach, iter, aoaStart, aoaEnd, aoaStep);
        if (!FileUtils::writeTextFile(polarInstr, instr)) {
            QMessageBox::critical(dlg, "File Error", "Unable to write XFOIL polar instruction file.");
            QFile::remove(inpPath);
            return;
        }

        calcBtn->setEnabled(false);
        exportBtn->setEnabled(false);
        clearBtn->setEnabled(false);
        calcBtn->setText("Calculating...");
        QApplication::processEvents();

        bool launched = XFoil::ProcessManager::instance().launch(appDir, polarInstr);

        calcBtn->setText("Calculate");
        calcBtn->setEnabled(true);

        if (!launched) { QFile::remove(inpPath); return; }

        if (!QFile::exists(polarOut)) {
            QMessageBox::warning(dlg, "Polar Not Generated",
                "XFOIL did not generate polar.txt.\nThe calculation may not have converged or the command sequence may need adjustment.");
            QFile::remove(inpPath);
            return;
        }

        QFile::copy(polarOut, polarCur);
        QFile::remove(polarOut);

        QVector<XFoil::PolarPoint> newPolar = XFoil::loadPolar(polarCur);
        if (newPolar.isEmpty()) {
            QMessageBox::warning(dlg, "No Polar Data", "The polar file was generated, but no valid data points were found.");
            QFile::remove(inpPath);
            return;
        }

        *polarData = newPolar;
        exportBtn->setEnabled(true);
        clearBtn->setEnabled(true);

        updateSummary();
        renderAllPlots();

        QFile::remove(inpPath);
        WindowUtils::bringWindowToFront(dlg);
    });

    dlg->show();
}

} // namespace Dialogs
