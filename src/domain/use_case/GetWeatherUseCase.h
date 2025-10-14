#ifndef DOMAIN_USE_CASE_GET_WEATHER_USE_CASE_H
#define DOMAIN_USE_CASE_GET_WEATHER_USE_CASE_H

#include "model/Forecast.h"
#include "repository/IWeatherRepository.h"

class GetWeatherUseCase {
    IWeatherRepository &m_repository;
public:
    explicit GetWeatherUseCase(IWeatherRepository &repo) : m_repository(repo) {}
    Forecast operator()(double lat, double lon, const QString &tz, int maxAge = 10) {
        return m_repository.get(lat, lon, tz, maxAge);
    }
};

#endif //DOMAIN_USE_CASE_GET_WEATHER_USE_CASE_H
