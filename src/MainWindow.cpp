#include "MainWindow.h"

#include "Dialogs/AirfoilTypeDialog.h"
#include "Dialogs/RepanelDialog.h"
#include "Dialogs/MorphDialog.h"
#include "Dialogs/FlapDialog.h"
#include "Dialogs/SolutionChoiceDialog.h"
#include "Dialogs/SinglePointDialog.h"
#include "Dialogs/PolarDialog.h"
#include "Dialogs/InfoDialog.h"

#include "Plot/PlotColors.h"
#include "Plot/PlotTransform.h"
#include "Utils/FileUtils.h"
#include "Utils/DatFile.h"
#include "XFoil/AirfoilFileRotation.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QPainter>
#include <QPixmap>
#include <QFont>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // clean / create temp dir
    const QString tempDir = AppPaths::tempDir();
    QDir dir(tempDir);
    if (dir.exists()) dir.removeRecursively();
    dir.mkpath(".");

    QWidget *central = new QWidget;
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QHBoxLayout *btnRow = new QHBoxLayout;
    const char *labels[] = {
        "Generate airfoil",
        "Import .dat",
        "Airfoil paneling",
        "Modify shape",
        "Add a flap",
        "Calculate solution",
        "Info"
    };
    const char *slotMethods[] = {
        SLOT(generateAirfoilClicked()),
        SLOT(importAirfoilClicked()),
        SLOT(repanelAirfoilClicked()),
        SLOT(morphAirfoilClicked()),
        SLOT(addFlapClicked()),
        SLOT(calculateSolutionClicked()),
        SLOT(showInfoClicked())
    };
    for (int i = 0; i < 7; ++i) {
        buttons[i] = new QPushButton(labels[i]);
        connect(buttons[i], SIGNAL(clicked()), this, slotMethods[i]);
        btnRow->addWidget(buttons[i]);
    }
    layout->addLayout(btnRow);

    plotArea = new ResizableWidget;
    plotArea->setMinimumHeight(400);
    plotArea->setStyleSheet("background-color: white; border: 1px solid #D1D5DB;");
    plotArea->onResize = [=]() { drawAirfoil(); };
    layout->addWidget(plotArea, 1);

    setWindowTitle("GUI Foil");
    resize(960, 680);
}

void MainWindow::drawAirfoil()
{
    const QString tempDir = AppPaths::tempDir();

    const DatFile::AirfoilData cur  = DatFile::load(tempDir + "/CurrentAirfoil.dat");
    const DatFile::AirfoilData prev = DatFile::load(tempDir + "/PreviousAirfoil.dat");
    if (cur.isEmpty())
        return;

    const int W = plotArea->width();
    const int H = plotArea->height();
    if (W <= 0 || H <= 0)
        return;

    const int margin = 45;
    double minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
    auto expand = [&](const QVector<QPointF> &pts) {
        for (auto &p : pts) {
            minX = qMin(minX, p.x()); maxX = qMax(maxX, p.x());
            minY = qMin(minY, p.y()); maxY = qMax(maxY, p.y());
        }
    };
    expand(cur.points);
    if (!prev.isEmpty()) expand(prev.points);

    const double pad = (maxY - minY) * 0.25 + 0.01;
    minY -= pad; maxY += pad;

    PlotTransform t;
    t.fit(minX, maxX, minY, maxY, W, H, margin);

    QPixmap pixmap(W, H);
    pixmap.fill(PlotColors::bg);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    PlotUtils::drawSimpleGrid(painter, t);

    if (!prev.isEmpty()) {
        painter.setPen(QPen(PlotColors::prevFoil, 1.5, Qt::DashLine));
        painter.drawPolygon(QPolygonF(PlotUtils::mapPolyline(t, prev.points)));
    }

    painter.setPen(QPen(PlotColors::curFoil, 2.0, Qt::SolidLine));
    painter.drawPolygon(QPolygonF(PlotUtils::mapPolyline(t, cur.points)));

    int lx = margin, ly = margin - 10;
    PlotUtils::drawLegendEntry(painter, lx, ly, PlotColors::curFoil, Qt::SolidLine, cur.name, 22, 18);
    if (!prev.isEmpty())
        PlotUtils::drawLegendEntry(painter, lx, ly, PlotColors::prevFoil, Qt::DashLine, prev.name, 22, 18);

    if (!plotLabel) plotLabel = new QLabel(plotArea);
    plotLabel->setGeometry(0, 0, W, H);
    plotLabel->setPixmap(pixmap);
    plotLabel->show();
}

void MainWindow::generateAirfoilClicked()
{
    Dialogs::showAirfoilTypeDialog(this, [this]() { drawAirfoil(); });
}

void MainWindow::importAirfoilClicked()
{
    const QString path = QFileDialog::getOpenFileName(this, "Import Airfoil", "",
                                                        "DAT files (*.dat);;All files (*)");
    if (path.isEmpty())
        return;

    const QString tempDir = AppPaths::tempDir();
    QDir().mkpath(tempDir);
    XFoil::rotateCurrentToPrevious(tempDir);
    QFile::copy(path, AppPaths::currentAirfoilDat());
    drawAirfoil();
}

void MainWindow::repanelAirfoilClicked()
{
    Dialogs::showRepanelDialog(this, [this]() { drawAirfoil(); });
}

void MainWindow::addFlapClicked()
{
    Dialogs::showFlapDialog(this, [this]() { drawAirfoil(); });
}

void MainWindow::morphAirfoilClicked()
{
    Dialogs::showMorphDialog(this, [this]() { drawAirfoil(); });
}

void MainWindow::showInfoClicked()
{
    Dialogs::showInfoDialog(this);
}

void MainWindow::calculateSolutionClicked()
{
    Dialogs::showSolutionChoiceDialog(this, &solutionDialog);
}

void MainWindow::openSinglePointSolutionDialog()
{
    Dialogs::showSinglePointDialog(this, &solutionDialog);
}

void MainWindow::openPolarSolutionDialog()
{
    Dialogs::showPolarDialog(this, &solutionDialog);
}
