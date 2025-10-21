#ifndef DOMAIN_MODEL_PROVIDER_H
#define DOMAIN_MODEL_PROVIDER_H

#include <QString>

enum class WeatherProviderId {
    OpenMeteo = 1,
};

enum class GeocoderProviderId {
    OpenMeteo = 1,
};

inline QString toString(const WeatherProviderId id) {
    switch (id) {
        case WeatherProviderId::OpenMeteo: return "Open-Meteo";
    }

    return "Open-Meteo";
}

inline WeatherProviderId toWeatherProvider(const QString &str) {
    if (str == "Open-Meteo" || str == "OpenMeteo") {
        return WeatherProviderId::OpenMeteo;
    }

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
