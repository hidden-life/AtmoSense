#include <QTime>
#include <QDir>

#include "IconMapper.h"
#include "Logger.h"
#include "cache/FileCacheStore.h"

QIcon IconMapper::iconFor(const int weatherCode, const bool isDarkTheme) {
    return map(weatherCode, isDarkTheme);
}

QIcon IconMapper::map(const int weatherCode, const bool isDarkTheme) {
    if (const QString path = resolveIconPath(weatherCode, isDarkTheme); QFile::exists(path)) {
        return QIcon(path);
    } else {
        Logger::warning("IconMapper: Missing icon: " + path);
        return QIcon(":/icons/unknown.png");
    }
}

QString IconMapper::resolveIconPath(int weatherCode, bool isDarkTheme) {
    // @todo Add here mapping according to input `isDarkTheme` (light or dark subfolder into `icons`)

    const QString weatherIconsPath = QString(":/icons/weather/%1/").arg(isDarkTheme ? "night" : "day");
    // https://open-meteo.com/en/docs
    // icons
    if (weatherCode == 0) return weatherIconsPath + "clear.png";
    if (weatherCode == 1 || weatherCode == 2) return weatherIconsPath + "partly_cloudy.png";
    if (weatherCode == 3) return weatherIconsPath + "cloudy.png";
    if (weatherCode == 45 || weatherCode == 48) return weatherIconsPath + "fog.png";
    if (weatherCode >= 51 && weatherCode <= 55) return weatherIconsPath + "mist.png";
    if (weatherCode >= 61 && weatherCode <= 65) return weatherIconsPath + "rain.png";
    if (weatherCode >= 71 && weatherCode <= 75) return weatherIconsPath + "snow.png";
    if (weatherCode == 77) return weatherIconsPath + "snow_grains.png";
    if (weatherCode >= 80 && weatherCode <= 82) return weatherIconsPath + "rain_thunder.png";
    if (weatherCode >= 95 && weatherCode <= 99) return weatherIconsPath + "thunder.png";

    return ":/icons/unknown.png";
}

bool IconMapper::isNightTime() {
    // simple rule: night is from 20:00 till 6:00
    const QTime now = QTime::currentTime();

    return (now < QTime(6, 0) || now >= QTime(20, 0));
}
