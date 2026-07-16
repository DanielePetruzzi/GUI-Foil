#include "FileUtils.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringList>

namespace AppPaths {

QString appDir()
{
    return QCoreApplication::applicationDirPath();
}

QString tempDir()
{
    return appDir() + "/temp";
}

QString appFile(const QString &name)
{
    return QDir(appDir()).filePath(name);
}

QString tempFile(const QString &name)
{
    return QDir(tempDir()).filePath(name);
}

QString currentAirfoilDat()
{
    return tempFile("CurrentAirfoil.dat");
}

QString previousAirfoilDat()
{
    return tempFile("PreviousAirfoil.dat");
}

QString instructionFile()
{
    return tempFile("xfoil_instr.txt");
}

} // namespace AppPaths

namespace FileUtils {

bool writeTextFile(const QString &path, const QString &content)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream out(&f);
    out << content;
    f.close();
    return true;
}

QString readTextFile(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();
    QString content = QString::fromLocal8Bit(f.readAll());
    f.close();
    return content;
}

void stripLinesAndMove(const QString &src, const QString &dst, int skip)
{
    QFile f(src);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QStringList lines;
    QTextStream in(&f);
    int n = 0;
    while (!in.atEnd()) {
        QString l = in.readLine();
        if (n++ >= skip)
            lines << l;
    }
    f.close();

    QFile::remove(dst);

    QFile out(dst);
    if (out.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream ts(&out);
        for (const auto &l : lines)
            ts << l << "\n";
    }

    QFile::remove(src);
}

} // namespace FileUtils
