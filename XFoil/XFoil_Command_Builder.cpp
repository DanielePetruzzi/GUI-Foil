#include "XFoil_Command_Builder.h"

XFoil_Command_Builder::XFoil_Command_Builder()
{
}

void XFoil_Command_Builder::clear()
{
    m_commands.clear();
}

void XFoil_Command_Builder::setSettings(
    const XFoil_Settings& settings)
{
    m_settings = settings;
}

void XFoil_Command_Builder::append(
    const QString& command)
{
    m_commands.append(command);
}

void XFoil_Command_Builder::loadAirfoil(
    const QString& fileName)
{
    append(QString("LOAD %1").arg(fileName));
}

void XFoil_Command_Builder::panel()
{
    if (!m_settings.repanel)
        return;

    append("PPAR");

    append(QString("N %1")
           .arg(m_settings.panelCount));

    append(QString("T %1")
           .arg(m_settings.panelTE, 0, 'f', 4));

    append(QString("R %1")
           .arg(m_settings.panelLE, 0, 'f', 4));

    append("");

    append("");
}

void XFoil_Command_Builder::enterOperMenu()
{
    append("OPER");
}

void XFoil_Command_Builder::enableViscous()
{
    if (!m_settings.viscous)
        return;

    append(QString("VISC %1")
           .arg(m_settings.reynolds, 0, 'f', 0));

    append(QString("MACH %1")
           .arg(m_settings.mach, 0, 'f', 4));

    append("VPAR");

    append(QString("N %1")
           .arg(m_settings.nCrit, 0, 'f', 2));

    append("");
}

void XFoil_Command_Builder::setIterations()
{
    append(QString("ITER %1")
           .arg(m_settings.iterations));
}

void XFoil_Command_Builder::startPolar(
    const QString& polarFile)
{
    append("PACC");
    append(polarFile);
    append("");
}

void XFoil_Command_Builder::stopPolar()
{
    append("PACC");
    append("");
    append("");
}

void XFoil_Command_Builder::setAlpha(
    double alpha)
{
    append(QString("ALFA %1")
           .arg(alpha, 0, 'f', 4));
}

void XFoil_Command_Builder::setLiftCoefficient(
    double cl)
{
    append(QString("CL %1")
           .arg(cl, 0, 'f', 4));
}

void XFoil_Command_Builder::writeCoordinates(
    const QString& fileName)
{
    append(QString("PWRT %1")
           .arg(fileName));
}

void XFoil_Command_Builder::writeCp(
    const QString& fileName)
{
    append(QString("CPWR %1")
           .arg(fileName));
}

void XFoil_Command_Builder::writeDump(
    const QString& fileName)
{
    append(QString("DUMP %1")
           .arg(fileName));
}

void XFoil_Command_Builder::quit()
{
    append("QUIT");
}

QString XFoil_Command_Builder::build() const
{
    return m_commands.join('\n') + '\n';
}