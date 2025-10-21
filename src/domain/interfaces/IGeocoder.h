#ifndef DOMAIN_INTERFACES_IGEOCODER_H
#define DOMAIN_INTERFACES_IGEOCODER_H

#include <vector>

#include "model/Location.h"

class IGeocoder {
public:
    virtual ~IGeocoder() = default;
    virtual std::vector<Location> search(const QString &query, const QString &lang, int count = 10) = 0;
    virtual std::vector<Location> search(const QString &text, int count = 10) = 0;
    virtual QString name() const = 0;
};

#endif //DOMAIN_INTERFACES_IGEOCODER_H
