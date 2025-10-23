#ifndef DOMAIN_MODEL_FORECAST_H
#define DOMAIN_MODEL_FORECAST_H

#include "Weather.h"
#include "AirQuality.h"

struct Forecast {
    Weather weather;
    std::vector<Weather> hourly;
    std::vector<Weather> daily;

    // air quality
    std::optional<AirQuality> currentAirQuality;
    std::vector<AirQuality> hourlyAirQuality;
};

#endif //DOMAIN_MODEL_FORECAST_H
