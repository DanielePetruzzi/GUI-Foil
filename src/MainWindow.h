#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QPointer>

#include "Widgets/ResizableWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void generateAirfoilClicked();
    void importAirfoilClicked();
    void repanelAirfoilClicked();
    void addFlapClicked();
    void morphAirfoilClicked();
    void showInfoClicked();
    void calculateSolutionClicked();
    void openSinglePointSolutionDialog();
    void openPolarSolutionDialog();

private:
    void drawAirfoil();

    QPushButton     *buttons[7] {};
    ResizableWidget  *plotArea  = nullptr;
    QLabel           *plotLabel = nullptr;
    QPointer<QDialog> solutionDialog = nullptr;
};
