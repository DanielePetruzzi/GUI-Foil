#include "XFoilCommandBuilder.h"

namespace XFoilCommands {

QString naca(const QString &code)
{
    return QString("naca\n%1\nsave\nOUTFILE\n\n").arg(code);
}

QString repanel(const QString &loadPath, int nPanels, const QString &savePath)
{
    return QString("load\n%1\n\nppar\nn\n%2\n\n\nsave\n%3\n\n")
        .arg(loadPath).arg(nPanels).arg(savePath);
}

QString flap(const QString &loadFile, double hingeX, double hingeY, double angleDeg, const QString &saveFile)
{
    return QString("load\n%1\n\ngdes\nflap\n%2\n%3\n%4\nx\n\nsave\n%5\n\n")
        .arg(loadFile)
        .arg(hingeX, 0, 'f', 4)
        .arg(hingeY, 0, 'f', 4)
        .arg(angleDeg, 0, 'f', 4)
        .arg(saveFile);
}

QString singlePoint(double aoa, double mach, double re, int iterations,
                     const QString &cpOutFile, const QString &accOutFile)
{
    if (mach == 0.0 && re == 0.0) {
        return QString("load\ninp.dat\n\noper\niter\n%1\nalfa\n%2\ncpwr\n%3\npacc\n%4\n\naseq\n%2\n%2\n1\n")
            .arg(iterations)
            .arg(aoa, 0, 'f', 4)
            .arg(cpOutFile)
            .arg(accOutFile);
    }
    if (mach == 0.0) {
        return QString("load\ninp.dat\n\noper\nvisc\n%1\niter\n%2\nalfa\n%3\ncpwr\n%4\npacc\n%5\n\naseq\n%3\n%3\n1\n")
            .arg((long long)re)
            .arg(iterations)
            .arg(aoa, 0, 'f', 4)
            .arg(cpOutFile)
            .arg(accOutFile);
    }
    if (re == 0.0) {
        return QString("load\ninp.dat\n\noper\nmach\n%1\niter\n%2\nalfa\n%3\ncpwr\n%4\npacc\n%5\n\naseq\n%3\n%3\n1\n")
            .arg(mach, 0, 'f', 4)
            .arg(iterations)
            .arg(aoa, 0, 'f', 4)
            .arg(cpOutFile)
            .arg(accOutFile);
    }
    return QString("load\ninp.dat\n\noper\nmach\n%1\nvisc\n%2\niter\n%3\nalfa\n%4\ncpwr\n%5\npacc\n%6\n\naseq\n%4\n%4\n1\n")
        .arg(mach, 0, 'f', 4)
        .arg((long long)re)
        .arg(iterations)
        .arg(aoa, 0, 'f', 4)
        .arg(cpOutFile)
        .arg(accOutFile);
}

QString polarSweep(double re, double mach, int iterations,
                    double aoaStart, double aoaEnd, double aoaStep)
{
    QString instr;

    instr += "load\n";
    instr += "inp.dat\n";
    instr += "\n";
    instr += "oper\n";

    instr += "visc\n";
    instr += QString("%1\n").arg((long long)re);

    if (mach > 0.0) {
        instr += "mach\n";
        instr += QString("%1\n").arg(mach, 0, 'f', 6);
    }

    instr += "iter\n";
    instr += QString("%1\n").arg(iterations);

    // Activate polar accumulation. XFOIL asks first for the polar save
    // file, then for an optional dump file (left empty here).
    instr += "pacc\n";
    instr += "polar.txt\n";
    instr += "\n";

    instr += "aseq\n";
    instr += QString("%1\n").arg(aoaStart, 0, 'f', 6);
    instr += QString("%1\n").arg(aoaEnd,   0, 'f', 6);
    instr += QString("%1\n").arg(aoaStep,  0, 'f', 6);

    // Deactivate polar accumulation.
    instr += "pacc\n";
    instr += "\n";

    return instr;
}

} // namespace XFoilCommands
