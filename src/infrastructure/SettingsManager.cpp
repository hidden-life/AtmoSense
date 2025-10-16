#include "SettingsManager.h"

SettingsManager::SettingsManager(QObject *parent) : QObject(parent), m_settings("Home Labs", "AtmoSense") {}

QString SettingsManager::language() const {
    return m_settings.value("app/language", "en").toString();
}

void SettingsManager::setLanguage(QString &language) {
    m_settings.setValue("app/language", language);
    emit settingsChanged();
}

QString SettingsManager::provider() const {
    return m_settings.value("weather/provider", "openmeteo").toString();
}

void SettingsManager::setProvider(const QString &provider) {
    m_settings.setValue("weather/provider", provider);
    emit settingsChanged();
}

double SettingsManager::latitude() const {
    return m_settings.value("weather/latitude", 50.4501).toDouble();
}

void SettingsManager::setLatitude(double latitude) {
    m_settings.setValue("weather/latitude", latitude);
}

double SettingsManager::longitude() const {
    return m_settings.value("weather/longitude", 30.5234).toDouble();
}

void SettingsManager::setLongitude(double longitude) {
    m_settings.setValue("weather/longitude", longitude);
}

int SettingsManager::refreshInterval() const {
    return m_settings.value("ui/refresh_interval", 15).toInt();
}

void SettingsManager::setRefreshInterval(int interval) {
    m_settings.setValue("ui/refresh_interval", interval);
    emit settingsChanged();
}

QString SettingsManager::locationName() const {
    return m_settings.value("weather/location_name", "Kyiv").toString();
}

void SettingsManager::setLocationName(QString &locationName) {
    m_settings.setValue("weather/location_name", locationName);
    emit settingsChanged();
}

QString SettingsManager::temperatureUnit() const {
    return m_settings.value("units/temperature", "C").toString();
}

void SettingsManager::setTemperatureUnit(const QString &unit) {
    m_settings.setValue("units/temperature", unit);
    emit settingsChanged();
}

QString SettingsManager::windSpeedUnit() const {
    return m_settings.value("units/wind_speed", "m/s").toString();
}

void SettingsManager::setWindSpeedUnit(const QString &windSpeed) {
    m_settings.setValue("units/wind_speed", windSpeed);
    emit settingsChanged();
}

QString SettingsManager::theme() const {
    return m_settings.value("app/theme", "auto").toString();
}

void SettingsManager::setTheme(const QString &theme) {
    m_settings.setValue("app/theme", theme);
    emit settingsChanged();
}

