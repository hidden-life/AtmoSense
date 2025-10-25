#ifndef DOMAIN_MODEL_PROVIDER_H
#define DOMAIN_MODEL_PROVIDER_H

#include <QString>

enum class WeatherProviderId {
    OpenMeteo = 1,
    OpenWeatherMap = 2,
};

enum class GeocoderProviderId {
    OpenMeteo = 1,
};

inline QString toString(const WeatherProviderId id) {
    switch (id) {
        case WeatherProviderId::OpenMeteo: return "open-meteo";
        case WeatherProviderId::OpenWeatherMap: return "open-weather-map";
    }

    return "open-meteo";
}

inline QString toDisplayName(const WeatherProviderId id) {
    switch (id) {
        case WeatherProviderId::OpenMeteo: return "Open-Meteo";
        case WeatherProviderId::OpenWeatherMap: return "Open Weather Map";
    }

    return "Open-Meteo";
}

inline WeatherProviderId toWeatherProvider(const QString &str) {
    if (str.toLower().contains("open-weather")) return WeatherProviderId::OpenWeatherMap;

    return WeatherProviderId::OpenMeteo;
}

inline QString toString(const GeocoderProviderId id) {
    switch (id) {
        case GeocoderProviderId::OpenMeteo: return "Open-Meteo Geocoder";
    }

    return "Open-Meteo Geocoder";
}

inline GeocoderProviderId toGeocoderProvider(const QString &str) {
    if (str == "Open-Meteo Geocoder" || str == "OpenMeteo Geocoder") {
        return GeocoderProviderId::OpenMeteo;
    }

    return GeocoderProviderId::OpenMeteo;
}

#endif //DOMAIN_MODEL_PROVIDER_H
