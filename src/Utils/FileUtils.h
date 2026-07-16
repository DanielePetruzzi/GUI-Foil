#pragma once

#include <QString>

// Centralizes the application/temp paths that were previously rebuilt with
// QCoreApplication::applicationDirPath() + "/temp" at dozens of call sites.
namespace AppPaths {

QString appDir();
QString tempDir();

// appDir() + "/" + name
QString appFile(const QString &name);
// tempDir() + "/" + name
QString tempFile(const QString &name);

// Well-known file names used throughout the app.
QString currentAirfoilDat();
QString previousAirfoilDat();
QString instructionFile();

} // namespace AppPaths

namespace FileUtils {

// Writes text to path (UTF/local8bit as appropriate), overwriting it.
// Returns false if the file could not be opened for writing.
bool writeTextFile(const QString &path, const QString &content);

// Reads the whole content of a text file, empty string if it can't be opened.
QString readTextFile(const QString &path);

// Reads src, drops the first `skip` lines, writes the remainder to dst and
// deletes src. Used to strip XFOIL header lines from result files before
// keeping them under a stable name (Results.txt / Pressure.txt).
void stripLinesAndMove(const QString &src, const QString &dst, int skip);

} // namespace FileUtils
