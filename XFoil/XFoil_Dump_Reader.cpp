#include "XFoil_Dump_Reader.h"

#include "../Utils/File_Utils.h"

#include <QStringList>

bool XFoil_Dump_Reader::read(
    const QString& fileName,
    QVector<XFoil_Dump_Point>& points,
    QString* error)
{
    points.clear();

    QString text;

    if (!File_Utils::readTextFile(
            fileName,
            text,
            error))
    {
        return false;
    }

    const QStringList lines =
        text.split('\n', Qt::SkipEmptyParts);

    for (const QString& rawLine : lines)
    {
        const QString line =
            rawLine.trimmed();

        if (line.isEmpty())
            continue;

        if (line.at(0).isLetter())
            continue;

        XFoil_Dump_Point point;

        if (parseLine(line, point))
            points.append(point);
    }

    return !points.isEmpty();
}

bool XFoil_Dump_Reader::parseLine(
    const QString& line,
    XFoil_Dump_Point& point)
{
    const QStringList values =
        line.simplified().split(' ');

    if (values.size() < 7)
        return false;

    bool ok = true;

    point.s = values[0].toDouble(&ok);
    if (!ok) return false;

    point.x = values[1].toDouble(&ok);
    if (!ok) return false;

    point.y = values[2].toDouble(&ok);
    if (!ok) return false;

    point.ueVinf = values[3].toDouble(&ok);
    if (!ok) return false;

    point.deltaStar = values[4].toDouble(&ok);
    if (!ok) return false;

    point.theta = values[5].toDouble(&ok);
    if (!ok) return false;

    point.cf = values[6].toDouble(&ok);
    if (!ok) return false;

    return true;
}