#ifndef INFRASTRUCTURE_SETTINGS_MANAGER_H
#define INFRASTRUCTURE_SETTINGS_MANAGER_H

#include <QSettings>

#include "model/Language.h"
#include "model/Theme.h"

class SettingsManager final : public QObject {
    Q_OBJECT

    QSettings m_settings;
public:
    explicit SettingsManager(QObject *parent = nullptr);
    // application language
    Language language() const;
    void setLanguage(Language lang);

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
    Theme theme() const;
    void setTheme(const Theme theme);

    // units
    QString temperatureUnit() const; // "C", "F"
    void setTemperatureUnit(const QString &unit);
    QString windSpeedUnit() const; // "m/s", "km/h"
    void setWindSpeedUnit(const QString &windSpeed);

    // show close prompt
    bool showClosePrompt() const { return m_showClosePrompt; }
    void setShowClosePrompt(bool value);

signals:
    void settingsChanged();

private:
    bool m_showClosePrompt = true;
};

#endif //INFRASTRUCTURE_SETTINGS_MANAGER_H
