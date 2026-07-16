#include "XFoilResultsReader.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QStringList>

namespace XFoil {

QVector<CpPoint> loadCpDistribution(const QString &path)
{
    QVector<CpPoint> pts;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return pts;

    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.contains('x') || line.contains("Cp"))
            continue;

        QStringList t = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (t.size() >= 3)
            pts.append({ t[0].toDouble(), t[1].toDouble(), t[2].toDouble() });
    }

    return pts;
}

QVector<PolarPoint> loadPolar(const QString &path)
{
    QVector<PolarPoint> pts;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return pts;

    QTextStream in(&f);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.isEmpty()) continue;
        if (line.startsWith("#")) continue;
        if (line.contains("alpha", Qt::CaseInsensitive)) continue;
        if (line.contains("------")) continue;
        if (line.contains("Mach", Qt::CaseInsensitive)) continue;
        if (line.contains("Re", Qt::CaseInsensitive)) continue;
        if (line.contains("xtr", Qt::CaseInsensitive)) continue;

        QStringList cols = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (cols.size() < 5) continue;

        bool ok0 = false, ok1 = false, ok2 = false, ok3 = false, ok4 = false;

        PolarPoint p;
        p.alpha = cols[0].toDouble(&ok0);
        p.cl    = cols[1].toDouble(&ok1);
        p.cd    = cols[2].toDouble(&ok2);
        p.cdp   = cols[3].toDouble(&ok3);
        p.cm    = cols[4].toDouble(&ok4);

        if (cols.size() >= 7) {
            p.topXtr = cols[5].toDouble();
            p.botXtr = cols[6].toDouble();
        }

        if (ok0 && ok1 && ok2 && ok3 && ok4)
            pts.append(p);
    }

    return pts;
}

Coefficients readClCm(const QString &resultsPath)
{
    Coefficients c;

    QFile rf(resultsPath);
    if (!rf.open(QIODevice::ReadOnly | QIODevice::Text))
        return c;

    QTextStream in(&rf);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList cols = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (cols.size() >= 6) {
            c.cl = cols[1].toDouble();
            if (c.cl > -0.002 && c.cl < 0.002)
                c.cl = 0;
            c.cm = cols[4].toDouble();
            c.valid = true;
            break;
        }
    }

    return c;
}

} // namespace XFoil
