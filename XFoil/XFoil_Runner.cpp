#include "XFoil_Runner.h"

#include "../Core/Airfoil_IO.h"
#include "../Utils/File_Utils.h"

#include "XFoil_Command_Builder.h"
#include "XFoil_Polar_Reader.h"
#include "XFoil_Cp_Reader.h"
#include "XFoil_Dump_Reader.h"


#include <QDir>
#include <QProcess>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

XFoil_Runner::XFoil_Runner()
{
    m_executable = "xfoil.exe";

    m_workingDirectory =
        QDir::currentPath() + "/workspace/xfoil";
}

void XFoil_Runner::setExecutable(
    const QString& executable)
{
    m_executable = executable;
}

void XFoil_Runner::setWorkingDirectory(
    const QString& directory)
{
    m_workingDirectory = directory;

    File_Utils::createDirectory(directory);
}

void XFoil_Runner::setSettings(
    const XFoil_Settings& settings)
{
    m_settings = settings;
}

QString XFoil_Runner::executable() const
{
    return m_executable;
}

QString XFoil_Runner::workingDirectory() const
{
    return m_workingDirectory;
}

const XFoil_Settings&
XFoil_Runner::settings() const
{
    return m_settings;
}

bool XFoil_Runner::runRepanel(
    const Airfoil& input,
    Airfoil& output,
    QString* error) const
{
    File_Utils::createDirectory(
        m_workingDirectory);

    const QString inputFile =
        QDir(m_workingDirectory).filePath(
            "input.dat");

    const QString outputFile =
        QDir(m_workingDirectory).filePath(
            "output.dat");

    const QString commandFile =
        QDir(m_workingDirectory).filePath(
            "commands.in");

    if (!Airfoil_IO::saveDat(
            inputFile,
            input,
            error))
    {
        return false;
    }

    XFoil_Command_Builder builder;

    builder.setSettings(m_settings);

    builder.loadAirfoil("input.dat");

    builder.panel();

    builder.writeCoordinates("output.dat");

    builder.quit();

    if (!File_Utils::writeTextFile(
            commandFile,
            builder.build(),
            error))
    {
        return false;
    }

    QString stdoutText;
    QString stderrText;

    if (!execute(commandFile,
                 &stdoutText,
                 &stderrText,
                 error))
    {
        return false;
    }

    if (!Airfoil_IO::loadDat(
            outputFile,
            output,
            error))
    {
        return false;
    }

    return true;
}

bool XFoil_Runner::execute(
    const QString& commandFile,
    QString* standardOutput,
    QString* standardError,
    QString* error) const
{
    QProcess process;

    process.setProgram(m_executable);

    process.setWorkingDirectory(
        m_workingDirectory);

    process.setStandardInputFile(
        commandFile);

#ifdef Q_OS_WIN

    process.setCreateProcessArgumentsModifier(

        [](QProcess::CreateProcessArguments* args)
        {
            args->flags |= CREATE_NO_WINDOW;
        });

#endif

    process.start();

    if (!process.waitForStarted())
    {
        if (error)
            *error = process.errorString();

        return false;
    }

    if (!process.waitForFinished(-1))
    {
        if (error)
            *error = process.errorString();

        return false;
    }

    if (standardOutput)
    {
        *standardOutput =
            QString::fromLocal8Bit(
                process.readAllStandardOutput());
    }

    if (standardError)
    {
        *standardError =
            QString::fromLocal8Bit(
                process.readAllStandardError());
    }

    if (process.exitStatus() !=
        QProcess::NormalExit)
    {
        if (error)
            *error = "XFoil terminated abnormally.";

        return false;
    }

    if (process.exitCode() != 0)
    {
        if (error)
        {
            if (standardError &&
                !standardError->isEmpty())
            {
                *error = *standardError;
            }
            else
            {
                *error = QString(
                    "XFoil exited with code %1.")
                    .arg(process.exitCode());
            }
        }

        return false;
    }

    return true;
}

bool XFoil_Runner::runAnalysis(
    const Airfoil& airfoil,
    double alpha,
    XFoil_Analysis_Result& result,
    QString* error) const
{
    File_Utils::createDirectory(
        m_workingDirectory);

    const QString inputFile =
        QDir(m_workingDirectory).filePath(
            "input.dat");

    const QString polarFile =
        QDir(m_workingDirectory).filePath(
            "polar.txt");

    const QString commandFile =
        QDir(m_workingDirectory).filePath(
            "commands.in");

    if (!Airfoil_IO::saveDat(
            inputFile,
            airfoil,
            error))
    {
        return false;
    }

    XFoil_Command_Builder builder;

    builder.setSettings(m_settings);

    builder.loadAirfoil("input.dat");

    builder.panel();

    builder.enterOperMenu();

    builder.enableViscous();

    builder.setIterations();

    builder.startPolar("polar.txt");

    builder.setAlpha(alpha);

    builder.stopPolar();

    builder.quit();

    if (!File_Utils::writeTextFile(
            commandFile,
            builder.build(),
            error))
    {
        return false;
    }

    QString stdoutText;
    QString stderrText;

    if (!execute(commandFile,
                 &stdoutText,
                 &stderrText,
                 error))
    {
        return false;
    }

    result.alpha = alpha;

    //
    // Il parsing del file polar verrà implementato
    // nella classe dedicata XFoil_Polar_Reader.
    //

    result.converged = true;

    return true;
}

bool XFoil_Runner::runPolar(
    const Airfoil& input,
    double alphaStart,
    double alphaEnd,
    double alphaStep,
    QVector<XFoil_Analysis_Result>& results,
    QString* error) const
{
    results.clear();

    const QString inputFile =
        QDir(m_workingDirectory).filePath("input.dat");

    const QString polarFile =
        QDir(m_workingDirectory).filePath("polar.txt");

    const QString commandFile =
        QDir(m_workingDirectory).filePath("commands.in");

    if (!Airfoil_IO::saveDat(inputFile,
                             input,
                             error))
    {
        return false;
    }

    XFoil_Command_Builder builder;

    builder.setSettings(m_settings);

    builder.loadAirfoil("input.dat");

    builder.panel();

    builder.enterOperMenu();

    builder.enableViscous();

    builder.setIterations();

    builder.startPolar("polar.txt");

    for (double alpha = alphaStart;
         alpha <= alphaEnd;
         alpha += alphaStep)
    {
        builder.setAlpha(alpha);
    }

    builder.stopPolar();

    builder.quit();

    if (!File_Utils::writeTextFile(commandFile,
                                   builder.build(),
                                   error))
    {
        return false;
    }

    if (!execute(commandFile,
                 nullptr,
                 nullptr,
                 error))
    {
        return false;
    }

    return XFoil_Polar_Reader::read(
        polarFile,
        results,
        error);
}

bool XFoil_Runner::runCp(
    const Airfoil& input,
    double alpha,
    QVector<QPointF>& cp,
    QString* error) const
{
    cp.clear();

    const QString inputFile =
        QDir(m_workingDirectory).filePath("input.dat");

    const QString cpFile =
        QDir(m_workingDirectory).filePath("cp.txt");

    const QString commandFile =
        QDir(m_workingDirectory).filePath("commands.in");

    if (!Airfoil_IO::saveDat(inputFile,
                             input,
                             error))
    {
        return false;
    }

    XFoil_Command_Builder builder;

    builder.setSettings(m_settings);

    builder.loadAirfoil("input.dat");

    builder.panel();

    builder.enterOperMenu();

    builder.enableViscous();

    builder.setIterations();

    builder.setAlpha(alpha);

    builder.writeCp("cp.txt");

    builder.quit();

    if (!File_Utils::writeTextFile(commandFile,
                                   builder.build(),
                                   error))
    {
        return false;
    }

    if (!execute(commandFile,
                 nullptr,
                 nullptr,
                 error))
    {
        return false;
    }

    return XFoil_Cp_Reader::read(
        cpFile,
        cp,
        error);
}

bool XFoil_Runner::runDump(
    const Airfoil& input,
    double alpha,
    QVector<XFoil_Dump_Point>& dump,
    QString* error) const
{
    dump.clear();

    const QString inputFile =
        QDir(m_workingDirectory).filePath("input.dat");

    const QString dumpFile =
        QDir(m_workingDirectory).filePath("dump.txt");

    const QString commandFile =
        QDir(m_workingDirectory).filePath("commands.in");

    if (!Airfoil_IO::saveDat(
            inputFile,
            input,
            error))
    {
        return false;
    }

    XFoil_Command_Builder builder;

    builder.setSettings(m_settings);

    builder.loadAirfoil("input.dat");

    builder.panel();

    builder.enterOperMenu();

    builder.enableViscous();

    builder.setIterations();

    builder.setAlpha(alpha);

    builder.writeDump("dump.txt");

    builder.quit();

    if (!File_Utils::writeTextFile(
            commandFile,
            builder.build(),
            error))
    {
        return false;
    }

    if (!execute(commandFile,
                 nullptr,
                 nullptr,
                 error))
    {
        return false;
    }

    return XFoil_Dump_Reader::read(
        dumpFile,
        dump,
        error);
}