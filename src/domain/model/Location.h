#ifndef DOMAIN_MODEL_LOCATION_H
#define DOMAIN_MODEL_LOCATION_H

#include <QString>

struct Location {
    QString name;
    QString country;
    QString region;
    QString timezone;

    double latitude = 0.0;
    double longitude = 0.0;
};

#endif //DOMAIN_MODEL_LOCATION_H
