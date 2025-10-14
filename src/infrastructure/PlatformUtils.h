#ifndef DOMAIN_INFRASTRUCTURE_PLATFORM_UTILS_H
#define DOMAIN_INFRASTRUCTURE_PLATFORM_UTILS_H

#include <QString>

class PlatformUtils {
    void setupAutostart(bool enabled);
    QString osName();
};

#endif //DOMAIN_INFRASTRUCTURE_PLATFORM_UTILS_H
