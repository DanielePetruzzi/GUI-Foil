#include "MainWindow.h"
#include "XFoil/XFoilProcessManager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QObject::connect(&app, &QCoreApplication::aboutToQuit, []() {
        XFoil::ProcessManager::instance().cleanupAll();
    });

    MainWindow w;
    w.show();

    return app.exec();
}
