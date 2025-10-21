#include "GeocoderFactory.h"

#include "open_meteo/OpenMeteoGeocoder.h"

std::shared_ptr<IGeocoder> GeocoderFactory::create(const QString &name, NetworkClient &client) {
    if (name == "OpenMeteo" || name == "Open-Meteo") {
        return std::make_shared<OpenMeteoGeocoder>(client);
    }

    throw std::runtime_error(("Unknown geocoder: " + name).toStdString());
}

std::map<QString, std::shared_ptr<IGeocoder>> GeocoderFactory::createAll(NetworkClient &client) {
    std::map<QString, std::shared_ptr<IGeocoder>> providers;

    auto geocoder = std::make_shared<OpenMeteoGeocoder>(client);
    providers.insert({ geocoder->name(), geocoder });

    // here add another providers bellow

    return providers;
}
