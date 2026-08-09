#include "logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

namespace Logger {
// Mutex ensures thread-safe logging. Multiple threads (e.g. worker threads)
// can write logs concurrently without corrupting the output file.
static QMutex s_logMutex;
static QString s_logFilePath;

void init(const QString &logFileName) {
    // Create a "logs" directory alongside the application executable
    QString logDir = QCoreApplication::applicationDirPath() + "/logs";
    QDir().mkpath(logDir);

    s_logFilePath = logDir + "/" + logFileName;

    // Write a separator marking the start of a new session
    QFile file(s_logFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "\n=================================================================\n";
        stream << "=== APPLICATION STARTED AT: "
               << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
               << " ===\n";
        stream << "=================================================================\n";
    }

    // Install this handler as the global Qt message handler
    qInstallMessageHandler(Logger::messageHandler);
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    // Lock the mutex so concurrent threads queue up while a write is in progress
    QMutexLocker locker(&s_logMutex);

    QFile file(s_logFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&file);

    // 1. Generate a millisecond-precision timestamp
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    // 2. Map the message type to a human-readable log level string
    QString levelStr;
    switch (type) {
    case QtDebugMsg:    levelStr = "[DEBUG]   "; break;
    case QtInfoMsg:     levelStr = "[INFO]    "; break;
    case QtWarningMsg:  levelStr = "[WARNING] "; break;
    case QtCriticalMsg: levelStr = "[CRITICAL]"; break;
    case QtFatalMsg:    levelStr = "[FATAL]   "; break;
    }

    // 3. Extract source file name and line number from the context (if available)
    QString fileInfo = "";
    if (context.file) {
        // Strip the absolute path, keep only the file basename for readability
        QString fullPath = QString::fromLatin1(context.file);
        fileInfo = QString(" (%1:%2, %3)")
                       .arg(QFileInfo(fullPath).fileName())
                       .arg(context.line)
                       .arg(context.function);
    }

    // 4. Format and write the log entry to the file
    // Example: 2026-07-06 18:25:01.123 [DEBUG]    ChessBoardWidget::resetBoard : Board initialized (ChessBoardWidget.cpp:52)
    stream << timestamp << " " << levelStr << " " << msg << fileInfo << "\n";

    // Flush immediately to disk to prevent log loss on unexpected crashes
    stream.flush();

    // On fatal errors, abort the application after logging
    if (type == QtFatalMsg) {
        abort();
    }

    fprintf(stderr, "%s %s %s\n", timestamp.toLocal8Bit().constData(), levelStr.toLocal8Bit().constData(), msg.toLocal8Bit().constData());
}
}