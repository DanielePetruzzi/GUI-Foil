#pragma once

#include <QString>

namespace XFoil {

// Moves CurrentAirfoil.dat -> PreviousAirfoil.dat (deleting any existing
// previous file first). Called before generating/importing/modifying an
// airfoil, so the plot can show a "previous" ghost outline.
void rotateCurrentToPrevious(const QString &tempDir);

// Same rotation pattern applied to the single-point solution result files
// (Results.txt / ResultsPrev.txt and Pressure.txt / PressurePrev.txt).
void rotateResultFiles(const QString &tempDir);

} // namespace XFoil
