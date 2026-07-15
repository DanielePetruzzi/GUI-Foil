#pragma once

#include <QString>

class Airfoil;

class Airfoil_IO
{
public:

    // DAT

    static bool loadDat(const QString& fileName,
                        Airfoil& airfoil,
                        QString* error = nullptr);

    static bool saveDat(const QString& fileName,
                        const Airfoil& airfoil,
                        QString* error = nullptr);

    // Utility

    static bool exists(const QString& fileName);

    static bool remove(const QString& fileName);

    static bool copy(const QString& source,
                     const QString& destination);

    static QString defaultAirfoilName(const QString& fileName);
};