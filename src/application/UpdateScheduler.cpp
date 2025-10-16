#include "UpdateScheduler.h"

#include "Logger.h"
#include "SettingsManager.h"

UpdateScheduler::UpdateScheduler(SettingsManager *settings, QObject *parent) : QObject(parent), m_settings(settings) {
    connect(&m_timer, &QTimer::timeout, this, &UpdateScheduler::onTimeout);
    connect(m_settings, &SettingsManager::settingsChanged, this, &UpdateScheduler::onSettingsChanged);

    restartTimer();
}

void UpdateScheduler::restartTimer() {
    int minutes = m_settings->refreshInterval();
    int ms = minutes * 60 * 1000;
    if (ms < 60000) {
        ms = 60000;
    }

    m_timer.start(ms);

    Logger::info("UpdateScheduler restarted with interval (min): " + minutes);
}

void UpdateScheduler::onTimeout() {
    Logger::info("Auto weather update triggered.");
    emit update();
}

void UpdateScheduler::onSettingsChanged() {
    restartTimer();
}