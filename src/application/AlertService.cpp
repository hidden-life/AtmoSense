#include "AlertService.h"

#include "Logger.h"
#include "SettingsManager.h"
#include "TrayService.h"
#include "UnitFormatter.h"
#include "model/UnitSystem.h"

AlertService::AlertService(SettingsManager *settings, TrayService *tray, QObject *parent) :
    QObject(parent),
    m_settings(settings),
    m_tray(tray),
    m_soundPlayer(new NotificationSoundPlayer(this)) {}

void AlertService::process(const Forecast &forecast) {
    if (!m_settings || !m_tray) {
        Logger::warning("AlertService: not initialized properly.");
        return;
    }

    maybeRain(forecast);
    maybeFreeze(forecast);
    maybeWind(forecast);
}

bool AlertService::willRainSoon(const Forecast &forecast) {
    // rule: if in 2 hours there is a rain code
    const int horizonHours = 2;
    for (int i = 0; i < (int)forecast.hourly.size() && i < horizonHours; ++i) {
        const Weather &w = forecast.hourly[i];

        // heuristic
        if (w.weatherCode >= 51 && w.weatherCode <= 67) {
            return true;
        }

        if (w.weatherCode >= 80 && w.weatherCode <= 82) {
            return true;
        }

        if (w.precipitationProbability.has_value() && *w.precipitationProbability >= 60.0) {
            return true;
        }

        if (w.precipitation.has_value() && *w.precipitation > 0.0) {
            return true;
        }
    }

    return false;
}

bool AlertService::isFreezingNow(const Forecast &forecast) {
    return forecast.weather.temperature <= 0.0;
}

bool AlertService::isWindyNow(const Forecast &forecast) {
    return forecast.weather.windSpeed >= 15.0;
}

bool AlertService::shouldThrottle(const std::optional<QDateTime> &lastAlert, int minutes) const {
    if (!lastAlert.has_value()) {
        return false;
    }

    const QDateTime now = QDateTime::currentDateTimeUtc();

    return lastAlert->secsTo(now) < (minutes * 60);
}


QString AlertService::describeCurrentTemperature(const Forecast &forecast) {
    const UnitSystem s = m_settings->unitSystem();
    return UnitFormatter::temperature(forecast.weather.temperature, s);
}

QString AlertService::describeCurrentWind(const Forecast &forecast) {
    const UnitSystem s = m_settings->unitSystem();
    return UnitFormatter::windSpeed(forecast.weather.windSpeed, s);
}


// concrete alerts
void AlertService::maybeRain(const Forecast &forecast) {
    if (!m_settings->notification(NotificationType::Rain)) return;

    // if there is no rain - skip
    if (!willRainSoon(forecast)) return;

    if (shouldThrottle(m_lastRain, 60)) return;

    const QString title = tr("Rain expected soon.");
    const QString body = tr("It rain within 2 hours. Temperature is: %1").arg(describeCurrentTemperature(forecast));

    Logger::info("AlertService: rain alert => tray");

    m_tray->showInfo(title, body);

    if (m_settings->notificationSoundEnabled()) {
        m_soundPlayer->play(NotificationType::Rain);
    }

    m_lastRain = QDateTime::currentDateTimeUtc();
}

void AlertService::maybeFreeze(const Forecast &forecast) {
    if (!m_settings->notification(NotificationType::Freeze)) return;

    if (!isFreezingNow(forecast)) return;

    if (shouldThrottle(m_lastFreeze, 120)) return;

    const QString title = tr("Freezing conditions.");
    const QString body = tr("Temperature is %1. Roads may be icy.").arg(describeCurrentTemperature(forecast));

    Logger::info("AlertService: freeze alert => tray");
    m_tray->showInfo(title, body);

    if (m_settings->notificationSoundEnabled()) {
        m_soundPlayer->play(NotificationType::Freeze);
    }

    m_lastFreeze = QDateTime::currentDateTimeUtc();
}

void AlertService::maybeWind(const Forecast &forecast) {
    if (!m_settings->notification(NotificationType::Wind)) return;

    if (!isWindyNow(forecast)) return;

    if (shouldThrottle(m_lastWind, 120)) return;

    const QString title = tr("Strong wind.");
    const QString body = tr("Wind speed is %1. Be careful outside.").arg(describeCurrentWind(forecast));

    Logger::info("AlertService: wind alert => tray");
    m_tray->showInfo(title, body);

    if (m_settings->notificationSoundEnabled()) {
        m_soundPlayer->play(NotificationType::Wind);
    }

    m_lastWind = QDateTime::currentDateTimeUtc();
}