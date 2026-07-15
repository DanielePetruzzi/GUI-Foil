#include "Airfoil_IO.h"
#include "Airfoil.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

bool Airfoil_IO::loadDat(const QString& fileName,
                        Airfoil& airfoil,
                        QString* error)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (error)
            *error = QString("Unable to open '%1'.").arg(fileName);

        return false;
    }

    QTextStream in(&file);

    airfoil.clear();

    bool firstLine = true;

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        if (firstLine)
        {
            airfoil.setName(line);
            firstLine = false;
            continue;
        }

        QStringList fields = line.simplified().split(' ', Qt::SkipEmptyParts);

        if (fields.size() < 2)
            continue;

        bool okX = false;
        bool okY = false;

        const double x = fields[0].toDouble(&okX);
        const double y = fields[1].toDouble(&okY);

        if (okX && okY)
            airfoil.addPoint(x, y);
    }

    if (airfoil.name().isEmpty())
        airfoil.setName(defaultAirfoilName(fileName));

    return !airfoil.isEmpty();
}

bool Airfoil_IO::saveDat(const QString& fileName,
                        const Airfoil& airfoil,
                        QString* error)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        if (error)
            *error = QString("Unable to write '%1'.").arg(fileName);

        return false;
    }

    QTextStream out(&file);

    out.setRealNumberNotation(QTextStream::FixedNotation);
    out.setRealNumberPrecision(8);

    out << airfoil.name() << '\n';

    for (const QPointF& p : airfoil.points())
    {
        out << QString("%1 %2")
               .arg(p.x(), 0, 'f', 8)
               .arg(p.y(), 0, 'f', 8)
            << '\n';
    }

    return true;
}

bool Airfoil_IO::exists(const QString& fileName)
{
    return QFile::exists(fileName);
}

bool Airfoil_IO::remove(const QString& fileName)
{
    return QFile::remove(fileName);
}

bool Airfoil_IO::copy(const QString& source,
                     const QString& destination)
{
    QFile::remove(destination);

    return QFile::copy(source, destination);
}

QString Airfoil_IO::defaultAirfoilName(const QString& fileName)
{
    return QFileInfo(fileName).completeBaseName();
}