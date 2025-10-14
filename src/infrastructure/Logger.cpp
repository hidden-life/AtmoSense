#include "Logger.h"

#include <QDebug>

namespace Logger {
    void info(const QString &message) { qInfo().noquote() << "[INFO] " << message << "\n"; }
    void warn(const QString &message) { qWarning().noquote() << "[WARNING] " << message << "\n"; }
    void error(const QString &message) { qCritical().noquote() << "[ERROR] " << message << "\n"; }
}