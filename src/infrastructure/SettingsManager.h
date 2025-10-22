#ifndef INFRASTRUCTURE_SETTINGS_MANAGER_H
#define INFRASTRUCTURE_SETTINGS_MANAGER_H

#include <QSettings>

#include "model/Language.h"
#include "model/Location.h"
#include "model/Theme.h"
#include "model/Provider.h"
#include "model/UnitSystem.h"

class SettingsManager final : public QObject {
    Q_OBJECT

    QSettings m_settings;
public:
    explicit SettingsManager(QObject *parent = nullptr);
    // application language
    [[nodiscard]]
    Language language() const;
    void setLanguage(Language lang);

    // weather provider
    [[nodiscard]]
    QString provider() const;
    void setProvider(const QString &provider);

    // location
    [[nodiscard]]
    QString locationName() const;
    void setLocationName(QString &location);
    [[nodiscard]]
    double latitude() const;
    void setLatitude(double latitude);
    [[nodiscard]]
    double longitude() const;
    void setLongitude(double longitude);
    // last location
    [[nodiscard]]
    Location lastLocation() const;
    void setLastLocation(const Location &location);

    // refresh
    [[nodiscard]]
    int refreshInterval() const;
    void setRefreshInterval(int minutes);

    // application theme: "auto", "light", "dark"
    [[nodiscard]]
    Theme theme() const;
    void setTheme(Theme theme);

    // units
    [[nodiscard]]
    QString temperatureUnit() const; // "C", "F"
    void setTemperatureUnit(const QString &unit);
    [[nodiscard]]
    QString windSpeedUnit() const; // "m/s", "km/h"
    void setWindSpeedUnit(const QString &windSpeed);

    // show close prompt
    [[nodiscard]]
    bool showClosePrompt() const { return m_showClosePrompt; }
    void setShowClosePrompt(bool value);

    // hourly display hours
    [[nodiscard]]
    int hourlyDisplayHours() const;
    void setHourlyDisplayHours(int hours);

    // providers
    [[nodiscard]]
    WeatherProviderId weatherProvider() const;
    void setWeatherProvider(WeatherProviderId weatherProvider);
    [[nodiscard]]
    GeocoderProviderId geocoderProvider() const;
    void setGeocoderProvider(GeocoderProviderId geocoderProvider);

    // unit system
    UnitSystem unitSystem() const;
    void setUnitSystem(UnitSystem sys);

    // recent locations
    std::vector<Location> recentLocations(int max = 8) const;
    void addRecentLocation(const Location &location); // deduplication and cutting using max

signals:
    void settingsChanged();
    void hourlyDisplayHoursChanged();

    // providers
    void weatherProviderChanged(WeatherProviderId id);
    void geocoderProviderChanged(GeocoderProviderId id);

private:
    bool m_showClosePrompt = true;
    QString m_lastLocation;
    double m_lastLatitude;
    double m_lastLongitude;
    QString m_lastTZ;
};

#endif //INFRASTRUCTURE_SETTINGS_MANAGER_H
