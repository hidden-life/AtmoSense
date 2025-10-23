#ifndef DOMAIN_MODEL_WEATHER_H
#define DOMAIN_MODEL_WEATHER_H

#include <QDateTime>

struct Weather {
    double temperature = 0.0;
    double windSpeed = 0.0;
    double humidity = 0.0;
    double pressure = 0.0;
    int weatherCode = 0;

    QDateTime timestamp;

    std::optional<double> precipitation;
    std::optional<double> precipitationProbability;
    std::optional<double> uvIndex;
};

#endif //DOMAIN_MODEL_WEATHER_H
