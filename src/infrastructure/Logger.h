#ifndef INFRASTRUCTURE_LOGGER_H
#define INFRASTRUCTURE_LOGGER_H

#include <QString>
#include <memory>

namespace spdlog {
    class logger;
}

class Logger {
public:
    static void init();
    static void shutdown();

    static void info(const QString &msg);
    static void warning(const QString &msg);
    static void error(const QString &msg);
    static void debug(const QString &msg);

    // 0 - trace
    // 1 - debug
    // 2 - info
    // 3 - warning
    // 4 - error
    static void setLevel(int level);

private:
    static std::shared_ptr<spdlog::logger> m_logger;
};

#endif //INFRASTRUCTURE_LOGGER_H
