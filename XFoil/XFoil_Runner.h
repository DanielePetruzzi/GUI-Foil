#pragma once

#include <QString>
#include <QVector>

#include "../Core/Airfoil.h"

#include "XFoil_Settings.h"
#include "XFoil_Analysis_Result.h"
#include "XFoil_Dump_Point.h"

class XFoil_Runner
{
public:

    XFoil_Runner();

    void setExecutable(const QString& executable);

    void setWorkingDirectory(const QString& directory);

    void setSettings(const XFoil_Settings& settings);

    QString executable() const;

    QString workingDirectory() const;

    const XFoil_Settings& settings() const;

    bool runRepanel(const Airfoil& input,
                    Airfoil& output,
                    QString* error = nullptr) const;

    bool runAnalysis(const Airfoil& input,
                     double alpha,
                     XFoil_Analysis_Result& result,
                     QString* error = nullptr) const;

    bool runPolar(const Airfoil& input,
                  double alphaStart,
                  double alphaEnd,
                  double alphaStep,
                  QVector<XFoil_Analysis_Result>& results,
                  QString* error = nullptr) const;

    bool runCp(const Airfoil& input,
               double alpha,
               QVector<QPointF>& cp,
               QString* error = nullptr) const;

    bool runDump(const Airfoil& input,
                 double alpha,
                 QVector<XFoil_Dump_Point>& dump,
                 QString* error = nullptr) const;

private:

    bool execute(const QString& commandFile,
                 QString* stdoutText = nullptr,
                 QString* stderrText = nullptr,
                 QString* error = nullptr) const;

private:

    QString m_executable;

    QString m_workingDirectory;

    XFoil_Settings m_settings;
};