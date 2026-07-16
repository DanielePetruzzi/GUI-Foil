#pragma once

#include <QString>
#include <QVector>
#include <QPointF>

// Reads/writes the simple airfoil .dat format used throughout the app:
// a name/header line followed by "x y" coordinate pairs.
namespace DatFile {

struct AirfoilData {
    QString name;
    QVector<QPointF> points;
    bool isEmpty() const { return points.isEmpty(); }
};

AirfoilData load(const QString &path);

// Writes header line (name) followed by points formatted as
// "  %10.6f  %10.6f", matching the format used by the CST/IGP/morph writers.
bool save(const QString &path, const QString &name, const QVector<QPointF> &points);

} // namespace DatFile
