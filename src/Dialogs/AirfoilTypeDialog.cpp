#include "AirfoilTypeDialog.h"
#include "ChildDialogFactory.h"
#include "Naca4Dialog.h"
#include "Naca5Dialog.h"
#include "CstDialog.h"
#include "IgpDialog.h"
#include "ParsecDialog.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>

namespace Dialogs {

void showAirfoilTypeDialog(QWidget *owner, const std::function<void()> &redraw)
{
    QDialog *dlg = new QDialog(owner);
    dlg->setWindowTitle("Select Airfoil Type");
    setupChildDialog(owner, dlg, 240, 190);

    QVBoxLayout *lay = new QVBoxLayout(dlg);
    QPushButton *naca4  = new QPushButton("NACA 4 digits");
    QPushButton *naca5  = new QPushButton("NACA 5 digits");
    QPushButton *cst    = new QPushButton("CST");
    QPushButton *igp    = new QPushButton("IGP");
    QPushButton *parsec = new QPushButton("PARSEC");
    lay->addWidget(naca4);
    lay->addWidget(naca5);
    lay->addWidget(cst);
    lay->addWidget(igp);
    lay->addWidget(parsec);

    QObject::connect(naca4,  &QPushButton::clicked, dlg, [=]() { showNaca4Dialog(owner, redraw);  dlg->accept(); });
    QObject::connect(naca5,  &QPushButton::clicked, dlg, [=]() { showNaca5Dialog(owner, redraw);  dlg->accept(); });
    QObject::connect(cst,    &QPushButton::clicked, dlg, [=]() { showCstDialog(owner, redraw);    dlg->accept(); });
    QObject::connect(igp,    &QPushButton::clicked, dlg, [=]() { showIgpDialog(owner, redraw);    dlg->accept(); });
    QObject::connect(parsec, &QPushButton::clicked, dlg, [=]() { showParsecDialog(owner, redraw); dlg->accept(); });

    dlg->show();
}

} // namespace Dialogs
