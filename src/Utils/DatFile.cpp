#include "DatFile.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QStringList>

namespace DatFile {

AirfoilData load(const QString &path)
{
    AirfoilData data;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return data;

    QTextStream in(&f);
    data.name = in.readLine().trimmed();

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() >= 2)
            data.points.append(QPointF(parts[0].toDouble(), parts[1].toDouble()));
    }

    return data;
}

bool save(const QString &path, const QString &name, const QVector<QPointF> &points)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream ts(&f);
    ts << name << "\n";
    for (const QPointF &p : points)
        ts << QString("  %1  %2\n").arg(p.x(), 10, 'f', 6).arg(p.y(), 10, 'f', 6);

    f.close();
    return true;
}

} // namespace DatFile
