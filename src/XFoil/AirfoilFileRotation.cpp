#include "AirfoilFileRotation.h"

#include <QFile>

namespace XFoil {

void rotateCurrentToPrevious(const QString &tempDir)
{
    const QString cur  = tempDir + "/CurrentAirfoil.dat";
    const QString prev = tempDir + "/PreviousAirfoil.dat";

    QFile::remove(prev);
    if (QFile::exists(cur))
        QFile::rename(cur, prev);
}

void rotateResultFiles(const QString &tempDir)
{
    const QString resCur   = tempDir + "/Results.txt";
    const QString resPrev  = tempDir + "/ResultsPrev.txt";
    const QString presCur  = tempDir + "/Pressure.txt";
    const QString presPrev = tempDir + "/PressurePrev.txt";

    QFile::remove(resPrev);
    QFile::rename(resCur, resPrev);

    QFile::remove(presPrev);
    QFile::rename(presCur, presPrev);
}

} // namespace XFoil
