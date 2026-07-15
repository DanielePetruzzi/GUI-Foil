#pragma once

#include <QString>
#include <QStringList>

#include "XFoil_Settings.h"

class XFoil_Command_Builder
{
public:

    XFoil_Command_Builder();

    void clear();

    void setSettings(const XFoil_Settings& settings);

    void loadAirfoil(const QString& fileName);

    void panel();

    void enterOperMenu();

    void enableViscous();

    void setIterations();

    void startPolar(const QString& polarFile);

    void stopPolar();

    void setAlpha(double alpha);

    void setLiftCoefficient(double cl);

    void writeCoordinates(const QString& fileName);

    void writeCp(const QString& fileName);

    void writeDump(const QString& fileName);

    void quit();

    QString build() const;

private:

    void append(const QString& command);

private:

    QStringList m_commands;

    XFoil_Settings m_settings;
};