#pragma once

#include <QString>

namespace AirfoilGen {

struct Naca5Params {
    int l;
    int p;
    int q;
    int xx; // thickness (%)
};

QString buildInstruction(const Naca5Params &params);

} // namespace AirfoilGen
