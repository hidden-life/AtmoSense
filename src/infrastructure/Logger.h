#ifndef INFRASTRUCTURE_LOGGER_H
#define INFRASTRUCTURE_LOGGER_H

#include <QString>

namespace Logger {
    void info(const QString &message);
    void warn(const QString &message);
    void error(const QString &message);
}

#endif //INFRASTRUCTURE_LOGGER_H
