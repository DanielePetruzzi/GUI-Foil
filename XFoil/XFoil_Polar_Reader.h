#pragma once

#include <QString>
#include <QVector>

#include "XFoil_Analysis_Result.h"

class XFoil_Polar_Reader
{
public:

    static bool read(const QString& fileName,
                     QVector<XFoil_Analysis_Result>& results,
                     QString* error = nullptr);

    static bool readLast(const QString& fileName,
                         XFoil_Analysis_Result& result,
                         QString* error = nullptr);

private:

    static bool parseLine(const QString& line,
                          XFoil_Analysis_Result& result);
};