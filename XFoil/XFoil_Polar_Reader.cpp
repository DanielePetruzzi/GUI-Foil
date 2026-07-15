#include "XFoil_Polar_Reader.h"

#include "../Utils/File_Utils.h"

#include <QStringList>

bool XFoil_Polar_Reader::read(
    const QString& fileName,
    QVector<XFoil_Analysis_Result>& results,
    QString* error)
{
    results.clear();

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
        const QString line = rawLine.trimmed();

        if (line.isEmpty())
            continue;

        if (!line.isEmpty() &&
            (line.at(0).isLetter() ||
             line.startsWith('-') ||
             line.startsWith("XFOIL")))
        {
            continue;
        }

        XFoil_Analysis_Result result;

        if (parseLine(line, result))
        {
            result.converged = true;
            results.append(result);
        }
    }

    return !results.isEmpty();
}

bool XFoil_Polar_Reader::readLast(
    const QString& fileName,
    XFoil_Analysis_Result& result,
    QString* error)
{
    QVector<XFoil_Analysis_Result> results;

    if (!read(fileName,
              results,
              error))
    {
        return false;
    }

    result = results.last();

    return true;
}

bool XFoil_Polar_Reader::parseLine(
    const QString& line,
    XFoil_Analysis_Result& result)
{
    const QStringList values =
        line.simplified().split(' ');

    if (values.size() < 7)
        return false;

    bool ok = true;

    result.alpha = values[0].toDouble(&ok);
    if (!ok) return false;

    result.cl = values[1].toDouble(&ok);
    if (!ok) return false;

    result.cd = values[2].toDouble(&ok);
    if (!ok) return false;

    result.cdp = values[3].toDouble(&ok);
    if (!ok) return false;

    result.cm = values[4].toDouble(&ok);
    if (!ok) return false;

    result.xtrTop = values[5].toDouble(&ok);
    if (!ok) return false;

    result.xtrBottom = values[6].toDouble(&ok);
    if (!ok) return false;

    return true;
}