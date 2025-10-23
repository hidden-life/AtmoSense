#ifndef DOMAIN_MODEL_AIR_QUALITY_H
#define DOMAIN_MODEL_AIR_QUALITY_H
#include <QDateTime>

struct AirQuality {
    QDateTime timestamp;
    std::optional<double> pm10;
    std::optional<double> pm2_5;
};

#endif //DOMAIN_MODEL_AIR_QUALITY_H
