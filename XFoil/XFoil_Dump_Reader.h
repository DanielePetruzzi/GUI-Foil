#pragma once

#include <QString>
#include <QVector>

#include "XFoil_Dump_Point.h"

class XFoil_Dump_Reader
{
public:

    static bool read(
        const QString& fileName,
        QVector<XFoil_Dump_Point>& points,
        QString* error = nullptr);

private:

    static bool parseLine(
        const QString& line,
        XFoil_Dump_Point& point);
};