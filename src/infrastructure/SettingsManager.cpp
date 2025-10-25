#include <QJsonObject>
#include <QJsonArray>

#include "SettingsManager.h"

#include "Logger.h"

SettingsManager::SettingsManager(QObject *parent) : QObject(parent), m_settings("Home Labs", "AtmoSense") {
    QSettings::setDefaultFormat(QSettings::IniFormat);
    Logger::info("SettingsManager initialized.");
    Logger::info("Loaded unit system: " + m_settings.value("units/system", "metric").toString());
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

UnitSystem SettingsManager::unitSystem() const {
    const QString val = m_settings.value("units/system", "metric").toString();
    return val.compare("imperial", Qt::CaseInsensitive) == 0 ? UnitSystem::Imperial : UnitSystem::Metric;
}

void SettingsManager::setUnitSystem(UnitSystem sys) {
    m_settings.setValue("units/system", toString(sys));
    m_settings.sync();

    emit settingsChanged();
}

static QJsonObject toJson(const Location &location) {
    QJsonObject obj;
    obj["name"] = location.name;
    obj["lat"] = location.latitude;
    obj["lon"] = location.longitude;
    obj["country"] = location.country;
    obj["region"] = location.region;
    obj["tz"] = location.timezone;

    return obj;
}

static Location fromJson(const QJsonObject &json) {
    Location location;
    location.name = json["name"].toString();
    location.latitude = json["lat"].toDouble();
    location.longitude = json["lon"].toDouble();
    location.country = json["country"].toString();
    location.region = json["region"].toString();
    location.timezone = json["tz"].toString();

    return location;
}

std::vector<Location> SettingsManager::recentLocations(const int max) const {
    const QByteArray raw = m_settings.value("location/recent", QByteArray()).toByteArray();
    std::vector<Location> locations;
    if (raw.isEmpty()) return locations;

    const QJsonDocument doc = QJsonDocument::fromJson(raw);
    if (!doc.isArray()) return locations;

    const QJsonArray arr = doc.array();
    locations.reserve(std::min<int>(max, arr.size()));
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i].isObject()) locations.push_back(fromJson(arr[i].toObject()));
    }

    return locations;
}

void SettingsManager::addRecentLocation(const Location &location) {
    auto list = recentLocations(50);
    auto same = [&](const Location &loc) {
        return qFuzzyCompare(loc.latitude, location.latitude) && qFuzzyCompare(loc.longitude, location.longitude);
    };

    list.erase(std::remove_if(list.begin(), list.end(), same), list.end());
    list.insert(list.begin(), location);

    if (list.size() > 8) list.resize(8);

    // serialize
    QJsonArray arr;
    for (const auto &l : list) arr.push_back(toJson(l));
    const QJsonDocument doc(arr);

    m_settings.setValue("location/recent", doc.toJson(QJsonDocument::Compact));
    m_settings.sync();

    emit settingsChanged();
}

bool SettingsManager::fetchAirQuality() const {
    return m_settings.value("data/fetch_air_quality", true).toBool();
}

void SettingsManager::setFetchAirQuality(bool enabled) {
    m_settings.setValue("data/fetch_air_quality", enabled);
    m_settings.sync();

    emit settingsChanged();
}

bool SettingsManager::fetchUV() const {
    return m_settings.value("data/fetch_uv", true).toBool();
}

void SettingsManager::setFetchUV(bool enabled) {
    m_settings.setValue("data/fetch_uv", enabled);
    m_settings.sync();

    emit settingsChanged();
}

bool SettingsManager::fetchPrecipitationProbability() const {
    return m_settings.value("data/fetch_precip_prob", true).toBool();
}

void SettingsManager::setFetchPrecipitationProbability(bool enabled) {
    m_settings.setValue("data/fetch_precip_prob", enabled);
    m_settings.sync();

    emit settingsChanged();
}

QString SettingsManager::openWeatherMapAPIKey() const {
    return m_settings.value("api/open_weather_map", "").toString();
}

void SettingsManager::setOpenWeatherMapAPIKey(const QString &key) {
    m_settings.setValue("api/open_weather_map", key.trimmed());
    m_settings.sync();

    emit apiKeyChanged();
    emit settingsChanged();
}

Locale SettingsManager::locale() const {
    return toLocale(m_settings.value("ui/locale", "system").toString());
}

void SettingsManager::setLocale(const Locale locale) {
    m_settings.setValue("ui/locale", toString(locale));
    m_settings.sync();

    emit settingsChanged();
}

bool SettingsManager::notification(const NotificationType type) const {
    return m_settings.value(QString("notifications/%1").arg(toString(type)), true).toBool();
}

void SettingsManager::setNotification(const NotificationType type, const bool isEnabled) {
    m_settings.setValue(QString("notifications/%1").arg(toString(type)), isEnabled);
    m_settings.sync();

    emit settingsChanged();
}

bool SettingsManager::notificationSoundEnabled() const {
    return m_settings.value("notifications/sound_enabled", true).toBool();
}

void SettingsManager::setNotificationSoundEnabled(bool isEnabled) {
    m_settings.setValue("notifications/sound_enabled", isEnabled);
    m_settings.sync();

    emit settingsChanged();
}

int SettingsManager::notificationSoundVolume() const {
    return m_settings.value("notifications/sound_volume", 80).toInt();
}

void SettingsManager::setNotificationSoundVolume(const int volume) {
    m_settings.setValue("notifications/sound_volume", volume);
    m_settings.sync();

    emit settingsChanged();
}

Theme SettingsManager::theme() const {
    const QString t = m_settings.value("ui/theme", static_cast<int>(Theme::System)).toString();
    return ThemeUtils::fromString(t);
}

void SettingsManager::setTheme(const Theme theme) {
    m_settings.setValue("ui/theme", ThemeUtils::toString(theme));
    m_settings.sync();

    emit settingsChanged();
}

