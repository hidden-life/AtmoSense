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
        case WeatherProviderId::OpenMeteo: return "open_meteo";
    }

    return "open_meteo";
}

inline WeatherProviderId toWeatherProvider(const QString &str) {
    if (str == "open_meteo") {
        return WeatherProviderId::OpenMeteo;
    }

    return WeatherProviderId::OpenMeteo;
}

inline QString toString(const GeocoderProviderId id) {
    switch (id) {
        case GeocoderProviderId::OpenMeteo: return "open_meteo";
    }

    return "open_meteo";
}

inline GeocoderProviderId toGeocoderProvider(const QString &str) {
    if (str == "open_meteo") {
        return GeocoderProviderId::OpenMeteo;
    }

    return GeocoderProviderId::OpenMeteo;
}

#endif //DOMAIN_MODEL_PROVIDER_H
