#ifndef DOMAIN_INFRASTRUCTURE_PLATFORM_UTILS_H
#define DOMAIN_INFRASTRUCTURE_PLATFORM_UTILS_H

#include <QObject>
#include <QString>

struct LocationInformation {
    QString country;
    QString region;
    QString city;
    double latitude = 0.0;
    double longitude = 0.0;

    bool isValid() {
        return latitude != 0.0 && longitude != 0.0;
    }

    QString displayName() const {
        if (!city.isEmpty() && !country.isEmpty()) return QString("%1, %2").arg(city, country);
        if (!region.isEmpty() && !country.isEmpty()) return QString("%1, %2").arg(region, country);

        return country.isEmpty() ? QObject::tr("Unknown") : country;
    }
};

class PlatformUtils {
public:
    void setupAutostart(bool enabled);
    QString osName();
    static std::optional<LocationInformation> detectLocation();
};

#endif //DOMAIN_INFRASTRUCTURE_PLATFORM_UTILS_H
