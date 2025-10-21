#ifndef DATA_OPEN_METEO_OPEN_METEO_GEOCODER_H
#define DATA_OPEN_METEO_OPEN_METEO_GEOCODER_H

#include "interfaces/IGeocoder.h"
#include "NetworkClient.h"

class OpenMeteoGeocoder : public IGeocoder {
    NetworkClient &m_client;
public:
    explicit OpenMeteoGeocoder(NetworkClient &client);
    std::vector<Location> search(const QString &query, const QString &lang, int count) override;
    std::vector<Location> search(const QString &text, int count) override;
    QString name() const override;
};

#endif //DATA_OPEN_METEO_OPEN_METEO_GEOCODER_H
