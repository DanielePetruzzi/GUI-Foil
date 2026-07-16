#include "Naca4Generator.h"
#include "../XFoil/XFoilCommandBuilder.h"

namespace AirfoilGen {

QString buildInstruction(const Naca4Params &params)
{
    const QString code = QString("%1%2%3")
        .arg(params.m)
        .arg(params.p)
        .arg(params.thickness, 2, 10, QChar('0'));

    return XFoilCommands::naca(code);
}

} // namespace AirfoilGen
