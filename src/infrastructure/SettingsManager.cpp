#include "SettingsManager.h"

SettingsManager::SettingsManager() : m_settings{} {}

QString SettingsManager::language() const {
    return m_settings.value("ui/language", "en").toString();
}

void SettingsManager::setLanguage(QString &language) {
    m_settings.setValue("ui/language", language);
}

QString SettingsManager::provider() const {
    return m_settings.value("ui/provider", "openmeteo").toString();
}

void SettingsManager::setProvider(QString &provider) {
    m_settings.setValue("ui/provider", provider);
}

double SettingsManager::lastLatitude() const {
    return m_settings.value("ui/lat", 50.4501).toDouble();
}

double SettingsManager::lastLongitude() const {
    return m_settings.value("ui/lon", 30.5234).toDouble();
}

void SettingsManager::setLastLocation(double latitude, double longitude) {
    m_settings.setValue("ui/lat", latitude);
    m_settings.setValue("ui/lon", longitude);
}

int SettingsManager::refreshInterval() const {
    return m_settings.value("ui/refresh_interval", 15).toInt();
}

void SettingsManager::setRefreshInterval(int interval) {
    m_settings.setValue("ui/refresh_interval", interval);
}