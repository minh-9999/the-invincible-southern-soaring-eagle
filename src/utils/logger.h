#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QtGlobal>

namespace Logger {
/**
 * @brief Initialize the logging system.
 * @param logFileName The desired log file name.
 */
void init(const QString &logFileName = "app_debug.log");

/**
 * @brief Global log message handler for Qt.
 * @param type The message type (debug, warning, critical, fatal, info).
 * @param context The message context (file, line, function).
 * @param msg The log message string.
 */
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
}

#endif // LOGGER_H
