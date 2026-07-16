#pragma once

#include <QString>

namespace AirfoilGen {

struct Naca4Params {
    int m;   // max camber (%)
    int p;   // camber position (x10)
    int thickness; // thickness (%)
};

// Builds the xfoil "naca <code>" instruction for the given digits.
// The generated airfoil is produced by xfoil itself (via
// XFoil::runXfoil), not computed locally.
QString buildInstruction(const Naca4Params &params);

} // namespace AirfoilGen
