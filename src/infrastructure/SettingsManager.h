#ifndef INFRASTRUCTURE_SETTINGS_MANAGER_H
#define INFRASTRUCTURE_SETTINGS_MANAGER_H

#include <QSettings>

class SettingsManager final : public QObject {
    Q_OBJECT

    QSettings m_settings;
public:
    SettingsManager(QObject *parent = nullptr);
    // application language
    QString language() const;
    void setLanguage(QString &language);

    // weather provider
    QString provider() const;
    void setProvider(const QString &provider);

    // location
    QString locationName() const;
    void setLocationName(QString &location);
    double latitude() const;
    void setLatitude(double latitude);
    double longitude() const;
    void setLongitude(double longitude);

    // refresh
    int refreshInterval() const;
    void setRefreshInterval(int minutes);

    // application theme: "auto", "light", "dark"
    QString theme() const;
    void setTheme(const QString &theme);

    // units
    QString temperatureUnit() const; // "C", "F"
    void setTemperatureUnit(const QString &unit);
    QString windSpeedUnit() const; // "m/s", "km/h"
    void setWindSpeedUnit(const QString &windSpeed);

signals:
    void settingsChanged();
};

#endif //INFRASTRUCTURE_SETTINGS_MANAGER_H
