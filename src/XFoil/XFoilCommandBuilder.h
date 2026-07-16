#pragma once

#include <QString>

// Builds the raw text instruction sequences fed to xfoil.exe on stdin.
// Each dialog used to build these strings inline; centralizing them here
// makes the XFOIL command syntax easy to find/change in one place.
namespace XFoilCommands {

// "naca\n<code>\nsave\nOUTFILE\n\n" — OUTFILE is substituted by
// XFoil::runXfoil() with temp/cur.dat.
QString naca(const QString &code);

// Loads loadPath, repanels to nPanels points, saves to savePath.
QString repanel(const QString &loadPath, int nPanels, const QString &savePath);

// Loads loadFile, applies a flap at (hingeX, hingeY) rotated by angleDeg,
// saves the result to saveFile.
QString flap(const QString &loadFile, double hingeX, double hingeY, double angleDeg, const QString &saveFile);

// Single operating point: load inp.dat, optional mach/visc(Re), fixed
// number of iterations, alpha sweep of a single point, Cp export to
// cpOutFile, CL/CM accumulation to accOutFile.
QString singlePoint(double aoa, double mach, double re, int iterations,
                     const QString &cpOutFile, const QString &accOutFile);

// Viscous polar sweep between aoaStart and aoaEnd (inclusive) with the
// given step, accumulating into polar.txt.
QString polarSweep(double re, double mach, int iterations,
                    double aoaStart, double aoaEnd, double aoaStep);

} // namespace XFoilCommands
