#pragma once

#include <QString>

class Airfoil;

class Airfoil_Repository
{
public:

    Airfoil_Repository();

    // Workspace

    void setWorkspace(const QString& directory);
    QString workspace() const;

    // Current workspace

    bool loadCurrent(Airfoil& airfoil,
                     QString* error = nullptr) const;

    bool loadPrevious(Airfoil& airfoil,
                      QString* error = nullptr) const;

    bool saveCurrent(const Airfoil& airfoil,
                     QString* error = nullptr) const;

    bool savePrevious(const Airfoil& airfoil,
                      QString* error = nullptr) const;

    /// Aggiorna automaticamente PreviousAirfoil.dat
    /// e salva il nuovo profilo come CurrentAirfoil.dat.
    bool commit(const Airfoil& airfoil,
                QString* error = nullptr) const;

    // Import / Export

    bool importAirfoil(const QString& fileName,
                       Airfoil& airfoil,
                       QString* error = nullptr) const;

    bool exportAirfoil(const QString& fileName,
                       const Airfoil& airfoil,
                       QString* error = nullptr) const;

    // Utilities

    bool hasCurrent() const;
    bool hasPrevious() const;

    QString currentAirfoilFile() const;
    QString previousAirfoilFile() const;

private:

    QString m_workspace;
};