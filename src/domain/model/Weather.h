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
};

#endif //DOMAIN_MODEL_WEATHER_H
