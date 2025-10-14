#ifndef CORE_APPLICATION_CONTEXT_H
#define CORE_APPLICATION_CONTEXT_H

#include <QDir>
#include <memory>

class NetworkClient;
class SettingsManager;
class ICacheStore;
class IWeatherProvider;
class IGeocoder;
class IWeatherRepository;

class ApplicationContext {
public:
    ApplicationContext();
    void init();

    std::shared_ptr<NetworkClient> network() const { return m_networkClient; }
    std::shared_ptr<SettingsManager> settings() const { return m_settings; }
    std::shared_ptr<ICacheStore> cache() const { return m_cache; }
    std::shared_ptr<IWeatherProvider> weatherProvider() const { return m_weatherProvider; }
    std::shared_ptr<IGeocoder> geocoder() const { return m_geocoder; }
    std::shared_ptr<IWeatherRepository> weatherRepository() const { return m_weatherRepository; }

private:
    QDir cacheDir() const;

    std::shared_ptr<NetworkClient> m_networkClient;
    std::shared_ptr<SettingsManager> m_settings;
    std::shared_ptr<ICacheStore> m_cache;
    std::shared_ptr<IWeatherProvider> m_weatherProvider;
    std::shared_ptr<IGeocoder> m_geocoder;
    std::shared_ptr<IWeatherRepository> m_weatherRepository;
};

#endif //CORE_APPLICATION_CONTEXT_H
