#pragma once

#include <QPointF>
#include <QString>
#include <QVector>

class XFoil_Cp_Reader
{
public:

    static bool read(const QString& fileName,
                     QVector<QPointF>& points,
                     QString* error = nullptr);
};