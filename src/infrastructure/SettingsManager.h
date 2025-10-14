#ifndef INFRASTRUCTURE_SETTINGS_MANAGER_H
#define INFRASTRUCTURE_SETTINGS_MANAGER_H

#include <QSettings>

class SettingsManager {
    QSettings m_settings;
public:
    SettingsManager();
    QString language() const;
    void setLanguage(QString &language);

    QString provider() const;
    void setProvider(QString &provider);

    double lastLatitude() const;
    double lastLongitude() const;
    void setLastLocation(double latitude, double longitude);

    int refreshInterval() const;
    void setRefreshInterval(int minutes);
};

#endif //INFRASTRUCTURE_SETTINGS_MANAGER_H
