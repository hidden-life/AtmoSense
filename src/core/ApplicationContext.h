#ifndef CORE_APPLICATION_CONTEXT_H
#define CORE_APPLICATION_CONTEXT_H

#include <QObject>
#include <QDir>
#include <memory>

#include "application/IconMapper.h"

class NetworkClient;
class SettingsManager;
class ICacheStore;
class IWeatherProvider;
class IGeocoder;
class IWeatherRepository;
class ThemeManager;
class UpdateScheduler;
class TranslationManager;
class IconMapper;

class ApplicationContext final : public QObject {
    Q_OBJECT
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
    std::shared_ptr<IWeatherRepository> weatherRepository() const { return m_weatherRepository; }

    [[nodiscard]]
    std::shared_ptr<ThemeManager> themeManager() const { return m_themeManager; }

    [[nodiscard]]
    std::shared_ptr<UpdateScheduler> updateScheduler() const { return m_updateScheduler; }

    [[nodiscard]]
    std::shared_ptr<TranslationManager> translation() const { return m_translationManager; }

    [[nodiscard]]
    std::shared_ptr<IconMapper> iconMapper() const { return m_iconMapper; }

    std::shared_ptr<IWeatherProvider> currentWeatherProvider() const { return m_currentWeatherProvider; };
    std::shared_ptr<IGeocoder> currentGeocoderProvider() const { return m_currentGeocoderProvider; };

    const std::map<QString, std::shared_ptr<IWeatherProvider>> &weatherProviders() const { return m_weatherProviders; }
    const std::map<QString, std::shared_ptr<IGeocoder>> &geocoderProviders() const { return m_geocoderProviders; }

    int weatherProviderCount() const;
    int geocoderProviderCount() const;

signals:
    void providersChanged();

private:
    [[nodiscard]]
    QDir cacheDir() const;

    std::shared_ptr<NetworkClient> m_networkClient;
    std::shared_ptr<SettingsManager> m_settings;
    std::shared_ptr<ICacheStore> m_cache;
    std::shared_ptr<IWeatherRepository> m_weatherRepository;
    std::shared_ptr<ThemeManager> m_themeManager;
    std::shared_ptr<UpdateScheduler> m_updateScheduler;
    std::shared_ptr<TranslationManager> m_translationManager;
    std::shared_ptr<IconMapper> m_iconMapper;

    std::map<QString, std::shared_ptr<IWeatherProvider>> m_weatherProviders;
    std::map<QString, std::shared_ptr<IGeocoder>> m_geocoderProviders;
    std::shared_ptr<IWeatherProvider> m_currentWeatherProvider;
    std::shared_ptr<IGeocoder> m_currentGeocoderProvider;

    void rebuildWeatherProvider();
    void rebuildGeocoder();
};

#endif //CORE_APPLICATION_CONTEXT_H
