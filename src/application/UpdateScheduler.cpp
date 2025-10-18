#include "UpdateScheduler.h"

#include <QDateTime>

#include "Logger.h"
#include "SettingsManager.h"

UpdateScheduler::UpdateScheduler(std::shared_ptr<SettingsManager> settings, QObject *parent) : QObject(parent), m_settings(std::move(settings)) {
    connect(&m_timer, &QTimer::timeout, this, &UpdateScheduler::onTimeout);
    connect(m_settings.get(), &SettingsManager::settingsChanged, this, &UpdateScheduler::onSettingsChanged);

    restartTimer();
}

void UpdateScheduler::pause() {
    if (m_timer.isActive()) {
        m_timer.stop();
        Logger::info("UpdateScheduler: pausing.");
    }
}

void UpdateScheduler::resume() {
    if (!m_timer.isActive()) {
        restartTimer();
        Logger::info("UpdateScheduler: resuming.");
    }
}

void UpdateScheduler::restartTimer() {
    // check if timer is not active, so on first start refresh will be done instantly
    if (!m_timer.isActive()) {
        QMetaObject::invokeMethod(this, &UpdateScheduler::onTimeout, Qt::QueuedConnection);
    }
    const int minutes = m_settings->refreshInterval();
    int ms = minutes * 60 * 1000;
    if (ms < 60000) {
        ms = 60000;
    }

    m_timer.start(ms);

    Logger::info("UpdateScheduler restarted with interval (min): " + minutes);
}

void UpdateScheduler::onTimeout() {
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (m_lastTrigger != 0 && now - m_lastTrigger < 30) {
        Logger::info("Skipping duplicate scheduler trigger.");
        return;
    }
    m_lastTrigger = now;

    Logger::info("Auto weather update triggered.");
    emit update();
}

void UpdateScheduler::onSettingsChanged() {
    static int lastInterval = -1;
    if (const int current = m_settings->refreshInterval(); current != lastInterval) {
        lastInterval = current;
        restartTimer();
    }
}