#include "Airfoil_Repository.h"

#include "Airfoil.h"
#include "Airfoil_IO.h"

#include "../Utils/Constants.h"

#include <QDir>

Airfoil_Repository::Airfoil_Repository()
    : m_workspace(QDir::currentPath())
{
    QDir().mkpath(m_workspace);
}

void Airfoil_Repository::setWorkspace(const QString& directory)
{
    m_workspace = directory;
    QDir().mkpath(m_workspace);
}

QString Airfoil_Repository::workspace() const
{
    return m_workspace;
}

bool Airfoil_Repository::loadCurrent(Airfoil& airfoil,
                                     QString* error) const
{
    return Airfoil_IO::loadDat(
        currentAirfoilFile(),
        airfoil,
        error);
}

bool Airfoil_Repository::loadPrevious(Airfoil& airfoil,
                                      QString* error) const
{
    return Airfoil_IO::loadDat(
        previousAirfoilFile(),
        airfoil,
        error);
}

bool Airfoil_Repository::saveCurrent(const Airfoil& airfoil,
                                     QString* error) const
{
    return Airfoil_IO::saveDat(
        currentAirfoilFile(),
        airfoil,
        error);
}

bool Airfoil_Repository::savePrevious(const Airfoil& airfoil,
                                      QString* error) const
{
    return Airfoil_IO::saveDat(
        previousAirfoilFile(),
        airfoil,
        error);
}

bool Airfoil_Repository::commit(const Airfoil& airfoil,
                                QString* error) const
{
    if (hasCurrent())
    {
        Airfoil_IO::remove(previousAirfoilFile());

        if (!Airfoil_IO::copy(currentAirfoilFile(),
                              previousAirfoilFile()))
        {
            if (error)
                *error = "Unable to update PreviousAirfoil.dat.";

            return false;
        }
    }

    return saveCurrent(airfoil, error);
}

bool Airfoil_Repository::importAirfoil(const QString& fileName,
                                       Airfoil& airfoil,
                                       QString* error) const
{
    return Airfoil_IO::loadDat(
        fileName,
        airfoil,
        error);
}

bool Airfoil_Repository::exportAirfoil(const QString& fileName,
                                       const Airfoil& airfoil,
                                       QString* error) const
{
    return Airfoil_IO::saveDat(
        fileName,
        airfoil,
        error);
}

bool Airfoil_Repository::hasCurrent() const
{
    return Airfoil_IO::exists(currentAirfoilFile());
}

bool Airfoil_Repository::hasPrevious() const
{
    return Airfoil_IO::exists(previousAirfoilFile());
}

QString Airfoil_Repository::currentAirfoilFile() const
{
    return QDir(m_workspace)
            .filePath(Constants::CurrentAirfoilFile);
}

QString Airfoil_Repository::previousAirfoilFile() const
{
    return QDir(m_workspace)
            .filePath(Constants::PreviousAirfoilFile);
}