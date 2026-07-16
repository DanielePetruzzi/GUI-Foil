#include "Naca5Generator.h"
#include "../XFoil/XFoilCommandBuilder.h"

namespace AirfoilGen {

QString buildInstruction(const Naca5Params &params)
{
    const QString code = QString("%1%2%3%4")
        .arg(params.l)
        .arg(params.p)
        .arg(params.q)
        .arg(params.xx, 2, 10, QChar('0'));

    return XFoilCommands::naca(code);
}

} // namespace AirfoilGen
