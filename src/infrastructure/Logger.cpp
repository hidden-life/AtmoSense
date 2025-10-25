#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::m_logger;

void Logger::init() {
    try {
        const auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        m_logger = std::make_shared<spdlog::logger>("AtmoSense", consoleSink);
        spdlog::register_logger(m_logger);

        m_logger->set_level(spdlog::level::info);
        m_logger->flush_on(spdlog::level::warn);
    } catch (const spdlog::spdlog_ex &e) {
        fprintf(stderr, "Logger initialization failed (%s)\n", e.what());
    }
}

void Logger::shutdown() {
    spdlog::shutdown();
}

void Logger::info(const QString &msg) {
    if (m_logger) m_logger->info(msg.toStdString());
}

void Logger::error(const QString &msg) {
    if (m_logger) m_logger->error(msg.toStdString());
}

void Logger::debug(const QString &msg) {
    if (m_logger) m_logger->debug(msg.toStdString());
}

void Logger::warning(const QString &msg) {
    if (m_logger) m_logger->warn(msg.toStdString());
}

void Logger::setLevel(int level) {
    if (!m_logger) return;

    m_logger->set_level(static_cast<spdlog::level::level_enum>(level));
}
