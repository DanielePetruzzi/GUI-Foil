#pragma once

#include <QString>
#include <QVector>
#include <QtGlobal>

// Owns the lifecycle of xfoil.exe child processes: launching them with an
// instruction file on stdin, tracking their PIDs so they can be force-killed
// on application exit, and writing diagnostic logs under temp/.
//
// This replaces the free functions and the anonymous-namespace PID vector
// that used to live at file scope in GUI_Foil.cpp.
namespace XFoil {

class ProcessManager {
public:
    static ProcessManager &instance();

    // Runs xfoil.exe from workDir, feeding it the content of instrFile on
    // stdin, waits for completion (60s timeout) and writes stdout/stderr/
    // error logs under <workDir>/temp. Shows a QMessageBox on failure.
    // Returns true if xfoil exited normally with code 0.
    bool launch(const QString &workDir, const QString &instrFile);

    void rememberProcess(qint64 pid);

    // Force-kills any xfoil processes still tracked (called on app exit).
    void cleanupAll();

private:
    ProcessManager() = default;
    QVector<qint64> m_pids;
};

// Convenience wrapper matching the original run_xfoil() free function:
// substitutes OUTFILE with temp/cur.dat in the instruction content, writes
// it to temp/xfoil_instr.txt, launches xfoil, then renames cur.dat to
// CurrentAirfoil.dat. Used by the NACA4/NACA5 generators.
void runXfoil(const QString &instrContent);

} // namespace XFoil
