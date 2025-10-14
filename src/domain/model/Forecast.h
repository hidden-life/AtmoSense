#ifndef DOMAIN_MODEL_FORECAST_H
#define DOMAIN_MODEL_FORECAST_H

#include "Weather.h"

struct Forecast {
    Weather weather;
    std::vector<Weather> hourly;
    std::vector<Weather> daily;
};

#endif //DOMAIN_MODEL_FORECAST_H
