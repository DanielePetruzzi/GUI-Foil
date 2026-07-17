#include "FlapDialog.h"
#include "ChildDialogFactory.h"
#include "../Plot/PlotTransform.h"
#include "../Plot/PlotColors.h"
#include "../Utils/DatFile.h"
#include "../Utils/FileUtils.h"
#include "../XFoil/XFoilProcessManager.h"
#include "../XFoil/XFoilCommandBuilder.h"
#include "../Widgets/ResizableLabel.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QtMath>

namespace Dialogs {

void showFlapDialog(QWidget *owner, const std::function<void()> &redraw)
{
    QDialog *dlg = new QDialog(owner);
    dlg->setWindowTitle("Add Flap");
    setupChildDialog(owner, dlg, 640, 480);

    QVBoxLayout *mainLay = new QVBoxLayout(dlg);

    auto addRow = [&](const QString &lbl, const QString &def) -> QLineEdit* {
        QHBoxLayout *h = new QHBoxLayout;
        h->addWidget(new QLabel(lbl));
        auto *e = new QLineEdit(def); h->addWidget(e);
        mainLay->addLayout(h); return e;
    };
    auto *xEdit = addRow("Hinge X (x/c):", "0.75");
    auto *yEdit = addRow("Hinge Y (y/c):", "0.0");
    auto *aEdit = addRow("Flap angle (deg):", "0.0");

    ResizableLabel *preview = new ResizableLabel;
    preview->setMinimumSize(400, 220);
    preview->setStyleSheet("background-color: white; border: 1px solid #D1D5DB;");
    mainLay->addWidget(preview, 1);

    auto renderPreview = [=](int W, int H) {
        if (W <= 0 || H <= 0) return;

        const double hx = xEdit->text().toDouble();
        const double hy = yEdit->text().toDouble();
        const double angle = aEdit->text().toDouble();
        const int margin = 30;

        const QVector<QPointF> pts = DatFile::load(AppPaths::currentAirfoilDat()).points;

        double minX = 0, maxX = 1, minY = -0.2, maxY = 0.2;
        for (auto &p : pts) {
            minX = qMin(minX, p.x()); maxX = qMax(maxX, p.x());
            minY = qMin(minY, p.y()); maxY = qMax(maxY, p.y());
        }
        const double pad = (maxY - minY) * 0.3;
        minY -= pad; maxY += pad;

        PlotTransform t;
        t.fit(minX, maxX, minY, maxY, W, H, margin);

        QPixmap px(W, H);
        px.fill(PlotColors::bg);
        QPainter pa(&px);
        pa.setRenderHint(QPainter::Antialiasing);

        PlotUtils::drawSimpleGrid(pa, t);

        pa.setPen(PlotColors::gridText);
        pa.setFont(QFont("Arial", 7));
        for (int i = 0; i <= 10; i += 2) {
            const double gx = minX + i * (maxX - minX) / 10.0;
            const QPointF p = t.toScreen(gx, minY);
            pa.drawText(p.x() - 10, p.y() + 12, QString::number(gx, 'f', 1));
        }

        if (!pts.isEmpty()) {
            pa.setPen(QPen(PlotColors::curFoil, 1.8));
            pa.drawPolygon(QPolygonF(PlotUtils::mapPolyline(t, pts)));
        }

        // hinge vertical
        pa.setPen(QPen(PlotColors::hingeV, 1.0, Qt::DashLine));
        pa.drawLine(t.toScreen(hx, maxY), t.toScreen(hx, minY));

        // flap direction arrow
        const double rad = -angle * M_PI / 180.0;
        const double fLen = (maxX - hx) * 1.2;
        const double ex = hx + fLen * std::cos(rad), ey = hy + fLen * std::sin(rad);
        pa.setPen(QPen(PlotColors::flapDir, 1.5, Qt::DashLine));
        pa.drawLine(t.toScreen(hx, hy), t.toScreen(ex, ey));

        // hinge dot
        pa.setPen(PlotColors::hingeV);
        pa.setBrush(PlotColors::hingeV);
        pa.drawEllipse(t.toScreen(hx, hy), 4, 4);

        preview->setPixmap(px);
    };

    preview->onResize = [=](int w, int h) { renderPreview(w, h); };
    auto triggerUpdate = [=]() { renderPreview(preview->width(), preview->height()); };
    QObject::connect(xEdit, &QLineEdit::textChanged, dlg, [=]() { triggerUpdate(); });
    QObject::connect(yEdit, &QLineEdit::textChanged, dlg, [=]() { triggerUpdate(); });
    QObject::connect(aEdit, &QLineEdit::textChanged, dlg, [=]() { triggerUpdate(); });
    triggerUpdate();

    QPushButton *applyBtn = new QPushButton("Apply");
    mainLay->addWidget(applyBtn);

    QObject::connect(applyBtn, &QPushButton::clicked, dlg, [=]() {
        const double hx = xEdit->text().toDouble();
        const double hy = yEdit->text().toDouble();
        const double angle = aEdit->text().toDouble();

        const QString appDir  = AppPaths::appDir();
        const QString tempDir = AppPaths::tempDir();

        const QString currentPath = AppPaths::currentAirfoilDat();
        const QString tmpPath     = AppPaths::appFile("tmp.dat");
        const QString curOutPath  = AppPaths::appFile("cur.dat");
        const QString prevPath    = AppPaths::previousAirfoilDat();

        QFile::remove(tmpPath);
        QFile::remove(curOutPath);
        QFile::copy(currentPath, tmpPath);

        QFile::remove(prevPath);
        QFile::rename(currentPath, prevPath);

        const QString instr = XFoilCommands::flap("tmp.dat", hx, hy, angle, "cur.dat");
        FileUtils::writeTextFile(AppPaths::instructionFile(), instr);

        if (!XFoil::ProcessManager::instance().launch(appDir, AppPaths::instructionFile())) {
            QFile::remove(tmpPath);
            return;
        }

        if (QFile::exists(curOutPath)) {
            if (QFile::exists(currentPath)) QFile::remove(currentPath);
            QFile::rename(curOutPath, currentPath);
        }

        const QString sign = (angle >= 0) ? "pos" : "neg";
        const QString flapTag = QString(" FLAP x%1 y%2 d%3%4")
            .arg(hx, 0, 'f', 2).arg(hy, 0, 'f', 2).arg(qAbs(angle), 0, 'f', 0).arg(sign);

        QFile datFile(currentPath);
        QString fileContent;
        if (datFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&datFile);
            const QString firstLine = in.readLine();
            fileContent = firstLine + flapTag + "\n" + in.readAll();
            datFile.close();
        }
        if (datFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&datFile);
            out << fileContent;
            datFile.close();
        }

        QFile::remove(tmpPath);

        redraw();
        dlg->accept();
    });

    dlg->show();
}

} // namespace Dialogs
