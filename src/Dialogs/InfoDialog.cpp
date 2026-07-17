#include "InfoDialog.h"
#include "ChildDialogFactory.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

namespace Dialogs {

void showInfoDialog(QWidget *owner)
{
    QDialog *dlg = new QDialog(owner);
    dlg->setWindowTitle("Info");
    setupChildDialog(owner, dlg, 340, 240);

    QVBoxLayout *lay = new QVBoxLayout(dlg);
    lay->setSpacing(4);

    auto addLbl = [&](const QString &t) { lay->addWidget(new QLabel(t)); };
    auto addLink = [&](const QString &url) {
        auto *l = new QLabel(QString("<a href='%1'>%1</a>").arg(url));
        l->setOpenExternalLinks(true);
        l->setCursor(Qt::PointingHandCursor);
        lay->addWidget(l);
    };

    addLbl("GUI-Foil \u00a9 2026 by Daniele Petruzzi");
    addLbl("Licensed under CC BY-SA 4.0");
    addLbl("");
    addLink("https://creativecommons.org/licenses/by-sa/4.0/");
    addLbl("");
    addLbl("Designed to work with XFoil by Mark Drela and Harold Youngren.");
    addLbl("");
    addLbl("Contact:");
    addLink("https://linkedin.com/in/daniele-petruzzi-74425a292");

    dlg->show();
}

} // namespace Dialogs
