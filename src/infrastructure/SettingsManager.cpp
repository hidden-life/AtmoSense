#include "SettingsManager.h"

SettingsManager::SettingsManager(QObject *parent) : QObject(parent), m_settings("Home Labs", "AtmoSense") {
    QSettings::setDefaultFormat(QSettings::IniFormat);
}

Language SettingsManager::language() const {
    const QString lang = m_settings.value("app/language", "en").toString();

    return LanguageUtils::fromCode(lang);
}

void SettingsManager::setLanguage(const Language lang) {
    m_settings.setValue("app/language", LanguageUtils::toCode(lang));
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

void SettingsManager::setLongitude(const double longitude) {
    m_settings.setValue("weather/longitude", longitude);
}

Location SettingsManager::lastLocation() const {
    Location location;
    location.name = m_settings.value("last_location/name").toString();
    location.latitude = m_settings.value("last_location/latitude").toDouble();
    location.longitude = m_settings.value("last_location/longitude").toDouble();
    location.timezone = m_settings.value("last_location/timezone").toString();

    return location;
}

void SettingsManager::setLastLocation(const Location &location) {
    m_settings.setValue("last_location/name", location.name);
    m_settings.setValue("last_location/latitude", location.latitude);
    m_settings.setValue("last_location/longitude", location.longitude);
    m_settings.setValue("last_location/timezone", location.timezone);

    m_settings.sync();
    emit settingsChanged();
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

void SettingsManager::setShowClosePrompt(const bool value) {
    if (m_showClosePrompt == value) return;

    m_showClosePrompt = value;
    emit settingsChanged();
}

int SettingsManager::hourlyDisplayHours() const {
    return m_settings.value("weather/hourly_display_hours", 24).toInt(); // default for 24 hours
}

void SettingsManager::setHourlyDisplayHours(int hours) {
    m_settings.setValue("weather/hourly_display_hours", hours);

    emit hourlyDisplayHoursChanged();
    emit settingsChanged();
}

WeatherProviderId SettingsManager::weatherProvider() const {
    const QString val = m_settings.value("providers/weather", "open_meteo").toString();

    return toWeatherProvider(val);
}

void SettingsManager::setWeatherProvider(WeatherProviderId id) {
    m_settings.setValue("providers/weather", toString(id));

    emit weatherProviderChanged(id);
    emit settingsChanged();
}

GeocoderProviderId SettingsManager::geocoderProvider() const {
    const QString val = m_settings.value("providers/geocoder", "open_meteo").toString();

    return toGeocoderProvider(val);
}

void SettingsManager::setGeocoderProvider(GeocoderProviderId id) {
    m_settings.setValue("providers/geocoder", toString(id));

    emit geocoderProviderChanged(id);
    emit settingsChanged();
}

Theme SettingsManager::theme() const {
    const QString t = m_settings.value("app/theme", "auto").toString();
    return ThemeUtils::fromString(t);
}

void SettingsManager::setTheme(const Theme theme) {
    m_settings.setValue("app/theme", ThemeUtils::toString(theme));
    emit settingsChanged();
}

