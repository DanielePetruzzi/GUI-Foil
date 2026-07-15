#include "File_Utils.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QStandardPaths>

bool File_Utils::exists(const QString& fileName)
{
    return QFile::exists(fileName);
}

bool File_Utils::remove(const QString& fileName)
{
    return QFile::remove(fileName);
}

bool File_Utils::copy(const QString& source,
                      const QString& destination)
{
    QFile::remove(destination);
    return QFile::copy(source, destination);
}

bool File_Utils::move(const QString& source,
                      const QString& destination)
{
    QFile::remove(destination);
    return QFile::rename(source, destination);
}

bool File_Utils::createDirectory(const QString& directory)
{
    return QDir().mkpath(directory);
}

bool File_Utils::writeTextFile(const QString& fileName,
                               const QString& text,
                               QString* error)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        if (error)
            *error = file.errorString();

        return false;
    }

    QTextStream out(&file);
    out << text;

    return true;
}

bool File_Utils::readTextFile(const QString& fileName,
                              QString& text,
                              QString* error)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (error)
            *error = file.errorString();

        return false;
    }

    QTextStream in(&file);

    text = in.readAll();

    return true;
}

QString File_Utils::fileName(const QString& path)
{
    return QFileInfo(path).fileName();
}

QString File_Utils::baseName(const QString& path)
{
    return QFileInfo(path).completeBaseName();
}

QString File_Utils::extension(const QString& path)
{
    return QFileInfo(path).suffix();
}

QString File_Utils::directory(const QString& path)
{
    return QFileInfo(path).absolutePath();
}

QString File_Utils::combine(const QString& directory,
                            const QString& file)
{
    return QDir(directory).filePath(file);
}

QString File_Utils::applicationDirectory()
{
    return QCoreApplication::applicationDirPath();
}

QString File_Utils::currentDirectory()
{
    return QDir::currentPath();
}

QString File_Utils::tempDirectory()
{
    return QDir::tempPath();
}