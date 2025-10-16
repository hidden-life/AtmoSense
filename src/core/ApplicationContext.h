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
class ThemeManager;
class UpdateScheduler;

class ApplicationContext {
public:
    ApplicationContext();
    void init();

    [[nodiscard]]
    std::shared_ptr<NetworkClient> network() const { return m_networkClient; }

    [[nodiscard]]
    std::shared_ptr<SettingsManager> settings() const { return m_settings; }

    [[nodiscard]]
    std::shared_ptr<ICacheStore> cache() const { return m_cache; }

    [[nodiscard]]
    std::shared_ptr<IWeatherProvider> weatherProvider() const { return m_weatherProvider; }

    [[nodiscard]]
    std::shared_ptr<IGeocoder> geocoder() const { return m_geocoder; }

    [[nodiscard]]
    std::shared_ptr<IWeatherRepository> weatherRepository() const { return m_weatherRepository; }

    [[nodiscard]]
    std::shared_ptr<ThemeManager> themeManager() const { return m_themeManager; }

    [[nodiscard]]
    std::shared_ptr<UpdateScheduler> updateScheduler() const { return m_updateScheduler; }

private:
    [[nodiscard]]
    QDir cacheDir() const;

    std::shared_ptr<NetworkClient> m_networkClient;
    std::shared_ptr<SettingsManager> m_settings;
    std::shared_ptr<ICacheStore> m_cache;
    std::shared_ptr<IWeatherProvider> m_weatherProvider;
    std::shared_ptr<IGeocoder> m_geocoder;
    std::shared_ptr<IWeatherRepository> m_weatherRepository;
    std::shared_ptr<ThemeManager> m_themeManager;
    std::shared_ptr<UpdateScheduler> m_updateScheduler;
};

#endif //CORE_APPLICATION_CONTEXT_H
