#pragma once

#include <QString>

class File_Utils
{
public:

    // File

    static bool exists(const QString& fileName);

    static bool remove(const QString& fileName);

    static bool copy(const QString& source,
                     const QString& destination);

    static bool move(const QString& source,
                     const QString& destination);

    // Directories

    static bool createDirectory(const QString& directory);

    // Text files

    static bool writeTextFile(const QString& fileName,
                              const QString& text,
                              QString* error = nullptr);

    static bool readTextFile(const QString& fileName,
                             QString& text,
                             QString* error = nullptr);

    // Path helpers

    static QString fileName(const QString& path);

    static QString baseName(const QString& path);

    static QString extension(const QString& path);

    static QString directory(const QString& path);

    static QString combine(const QString& directory,
                           const QString& file);

    // Standard locations

    static QString applicationDirectory();

    static QString currentDirectory();

    static QString tempDirectory();
};