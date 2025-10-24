#ifndef APPLICATION_ALERT_SERVICE_H
#define APPLICATION_ALERT_SERVICE_H

#include <QObject>

#include "model/Forecast.h"

class SettingsManager;
class TrayService;

class AlertService final : public QObject {
    Q_OBJECT
public:
    AlertService(SettingsManager *settings, TrayService *tray, QObject *parent = nullptr);
    void process(const Forecast &forecast);

private:
    SettingsManager *m_settings;
    TrayService *m_tray;

    std::optional<QDateTime> m_lastRain;
    std::optional<QDateTime> m_lastFreeze;
    std::optional<QDateTime> m_lastWind;

    // internal checks
    void maybeRain(const Forecast &forecast);
    void maybeFreeze(const Forecast &forecast);
    void maybeWind(const Forecast &forecast);

    bool shouldThrottle(const std::optional<QDateTime> &lastAlert, int minutes) const;

    static bool willRainSoon(const Forecast &forecast);
    static bool isFreezingNow(const Forecast &forecast);
    static bool isWindyNow(const Forecast &forecast);

    QString describeCurrentTemperature(const Forecast &forecast);
    QString describeCurrentWind(const Forecast &forecast);
};

#endif //APPLICATION_ALERT_SERVICE_H
