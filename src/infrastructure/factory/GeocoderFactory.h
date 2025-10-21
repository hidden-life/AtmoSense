#ifndef INFRASTRUCTURE_FACTORY_GEOCODER_FACTORY_H
#define INFRASTRUCTURE_FACTORY_GEOCODER_FACTORY_H

#include <memory>

#include "NetworkClient.h"
#include "interfaces/IGeocoder.h"
#include "model/Provider.h"

class GeocoderFactory final {
public:
    static std::shared_ptr<IGeocoder> create(const QString &name, NetworkClient &client);
    static std::map<QString, std::shared_ptr<IGeocoder>> createAll(NetworkClient &client);
};

#endif //INFRASTRUCTURE_FACTORY_GEOCODER_FACTORY_H
