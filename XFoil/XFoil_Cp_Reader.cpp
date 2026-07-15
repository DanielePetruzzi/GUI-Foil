#include "XFoil_Cp_Reader.h"

#include "../Utils/File_Utils.h"

#include <QStringList>

bool XFoil_Cp_Reader::read(
    const QString& fileName,
    QVector<QPointF>& points,
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

        const QStringList values =
            line.simplified().split(' ');

        if (values.size() < 3)
            continue;

        bool ok1;
        bool ok2;

        const double x =
            values[0].toDouble(&ok1);

        const double cp =
            values[2].toDouble(&ok2);

        if (!ok1 || !ok2)
            continue;

        points.append(
            QPointF(x, cp));
    }

    return !points.isEmpty();
}