#include "XFoilProcessManager.h"
#include "../Utils/FileUtils.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QMessageBox>

namespace XFoil {

ProcessManager &ProcessManager::instance()
{
    static ProcessManager mgr;
    return mgr;
}

void ProcessManager::rememberProcess(qint64 pid)
{
    if (pid > 0 && !m_pids.contains(pid))
        m_pids.append(pid);
}

void ProcessManager::cleanupAll()
{
    const QVector<qint64> pids = m_pids;
    m_pids.clear();

    for (qint64 pid : pids) {
        if (pid <= 0)
            continue;
#ifdef Q_OS_WIN
        QProcess::execute("taskkill", QStringList() << "/PID" << QString::number(pid) << "/T" << "/F");
#else
        QProcess::execute("kill", QStringList() << "-TERM" << QString::number(pid));
#endif
    }
}

bool ProcessManager::launch(const QString &workDir, const QString &instrFile)
{
    const QString exe = QDir(workDir).filePath("xfoil.exe");
    const QString tempDir = QDir(workDir).filePath("temp");
    QDir().mkpath(tempDir);

    auto writeLog = [&](const QString &name, const QByteArray &data) {
        QFile logFile(QDir(tempDir).filePath(name));
        if (logFile.open(QIODevice::WriteOnly | QIODevice::Text))
            logFile.write(data);
    };

    if (!QFile::exists(exe)) {
        writeLog("xfoil_error.txt", QString("xfoil.exe not found: %1\n").arg(exe).toLocal8Bit());
        QMessageBox::critical(nullptr, "XFoil error",
            QString("xfoil.exe non trovato:\n%1\n\nMettilo nella stessa cartella dell'eseguibile della GUI.").arg(exe));
        return false;
    }

    QFile inputFile(instrFile);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        writeLog("xfoil_error.txt", QString("Cannot open instruction file: %1\n").arg(instrFile).toLocal8Bit());
        QMessageBox::critical(nullptr, "XFoil error",
            QString("Impossibile aprire il file istruzioni XFoil:\n%1").arg(instrFile));
        return false;
    }

    QString instr = QString::fromLocal8Bit(inputFile.readAll());
    inputFile.close();

    if (!instr.trimmed().endsWith("quit", Qt::CaseInsensitive))
        instr += "\nquit\n";

    writeLog("xfoil_last_input.txt", instr.toLocal8Bit());

    QProcess proc;
    proc.setWorkingDirectory(workDir);
    proc.setProcessChannelMode(QProcess::SeparateChannels);

#ifdef Q_OS_WIN
    proc.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *a) {
        a->flags |= CREATE_NO_WINDOW;
    });
#endif

    proc.start(exe);
    if (!proc.waitForStarted(5000)) {
        const QString err = QString("Cannot start xfoil.exe: %1\nExecutable: %2\n")
                                .arg(proc.errorString(), exe);
        writeLog("xfoil_error.txt", err.toLocal8Bit());
        QMessageBox::critical(nullptr, "XFoil error", err);
        return false;
    }

    rememberProcess(proc.processId());

    proc.write(instr.toLocal8Bit());
    proc.closeWriteChannel();

    if (!proc.waitForFinished(60000)) {
        proc.kill();
        proc.waitForFinished(3000);
        const QString err = "XFoil timeout: processo terminato dopo 60 s. Vedi temp/xfoil_last_input.txt.\n";
        writeLog("xfoil_error.txt", err.toLocal8Bit());
        QMessageBox::critical(nullptr, "XFoil timeout", err);
        return false;
    }

    const QByteArray out = proc.readAllStandardOutput();
    const QByteArray err = proc.readAllStandardError();
    writeLog("xfoil_stdout.txt", out);
    writeLog("xfoil_stderr.txt", err);

    if (proc.exitStatus() != QProcess::NormalExit || proc.exitCode() != 0) {
        const QString msg = QString("XFoil terminato con errore. Exit code: %1\n\nVedi:\n%2\n%3")
                                .arg(proc.exitCode())
                                .arg(QDir(tempDir).filePath("xfoil_stdout.txt"))
                                .arg(QDir(tempDir).filePath("xfoil_stderr.txt"));
        writeLog("xfoil_error.txt", msg.toLocal8Bit());
        QMessageBox::warning(nullptr, "XFoil warning", msg);
        return false;
    }

    return true;
}

void runXfoil(const QString &instrContent)
{
    const QString appDir  = AppPaths::appDir();
    const QString tempDir = AppPaths::tempDir();
    const QString instrFile = AppPaths::instructionFile();

    QString instr = instrContent;
    instr.replace("OUTFILE", tempDir + "/cur.dat");

    FileUtils::writeTextFile(instrFile, instr);

    if (!ProcessManager::instance().launch(appDir, instrFile))
        return;

    const QString curPath   = tempDir + "/cur.dat";
    const QString finalPath = AppPaths::currentAirfoilDat();
    if (QFile::exists(curPath)) {
        QFile::remove(finalPath);
        QFile::rename(curPath, finalPath);
    }
}

} // namespace XFoil
