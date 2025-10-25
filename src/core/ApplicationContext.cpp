#include <QStandardPaths>

#include "ApplicationContext.h"

#include "Logger.h"
#include "NetworkClient.h"
#include "SettingsManager.h"
#include "ThemeManager.h"
#include "TranslationManager.h"
#include "application/IconMapper.h"
#include "application/UpdateScheduler.h"
#include "cache/FileCacheStore.h"
#include "factory/GeocoderFactory.h"
#include "factory/WeatherProviderFactory.h"
#include "open_meteo/OpenMeteoWeatherProvider.h"
#include "open_meteo/OpenMeteoGeocoder.h"
#include "repository/WeatherRepository.h"

ApplicationContext::ApplicationContext() = default;

int ApplicationContext::weatherProviderCount() const {
    return m_weatherProviders.size();
}

int ApplicationContext::geocoderProviderCount() const {
    return m_geocoderProviders.size();
}

QDir ApplicationContext::cacheDir() const {
    const QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return dir;
}

void ApplicationContext::rebuildWeatherProvider() {
    const QString selected = toString(m_settings->weatherProvider());
    if (const auto it = m_weatherProviders.find(selected); it != m_weatherProviders.end()) {
        m_currentWeatherProvider = it->second;
        Logger::info("Weather provider switched: " + selected);
    } else if (!m_weatherProviders.empty()) {
        m_currentWeatherProvider = m_weatherProviders.begin()->second;
        Logger::warning("Unknown weather provider <" + selected + ">, fallback to default.");
    }

    if (m_weatherRepository && m_currentWeatherProvider) {
        m_weatherRepository->setProvider(m_currentWeatherProvider);
    }
}

void ApplicationContext::rebuildGeocoder() {
    const QString selected = toString(m_settings->geocoderProvider());
    if (const auto it = m_geocoderProviders.find(selected); it != m_geocoderProviders.end()) {
        m_currentGeocoderProvider = it->second;
        Logger::info("Geocoder provider switched: " + selected);
    } else if (!m_geocoderProviders.empty()) {
        m_currentGeocoderProvider = m_geocoderProviders.begin()->second;
        Logger::warning("Unknown geocoder <" + selected + ">, fallback to default.");
    }
}

void ApplicationContext::init() {
    m_networkClient = std::make_shared<NetworkClient>();
    m_settings = std::make_shared<SettingsManager>();
    m_cache = std::make_shared<FileCacheStore>(cacheDir());

    // set application locale
    switch (m_settings->locale()) {
        case Locale::English: QLocale::setDefault(QLocale(QLocale::English)); break;
        case Locale::Ukrainian: QLocale::setDefault(QLocale(QLocale::Ukrainian)); break;
        default: QLocale::setDefault(QLocale(QLocale::system())); break;
    }

    // create providers using factories
    m_weatherProviders = WeatherProviderFactory::createAll(*m_networkClient, *m_settings);
    m_geocoderProviders = GeocoderFactory::createAll(*m_networkClient);

    // first creation based on settings and set to m_currentWeatherProvider & m_currentGeocoderProvider
    rebuildWeatherProvider();
    rebuildGeocoder();

    m_weatherRepository = std::make_shared<WeatherRepository>(m_currentWeatherProvider, *m_cache);

    m_themeManager = std::make_shared<ThemeManager>(m_settings);
    m_updateScheduler = std::make_shared<UpdateScheduler>(m_settings);

    m_translationManager = std::make_shared<TranslationManager>(m_settings);
    m_translationManager->apply();

    m_iconMapper = std::make_shared<IconMapper>();

    connect(m_settings.get(), &SettingsManager::weatherProviderChanged, [this](WeatherProviderId id) {
        rebuildWeatherProvider();
        // "send" new provider to repository
        if (m_weatherRepository && m_currentWeatherProvider) m_weatherRepository->setProvider(m_currentWeatherProvider);

        emit providersChanged();
    });

    connect(m_settings.get(), &SettingsManager::geocoderProviderChanged, [this](GeocoderProviderId) {
        rebuildGeocoder();
        emit providersChanged();
    });

    connect(m_settings.get(), &SettingsManager::apiKeyChanged, this, [this] {
        Logger::info("ApplicationContext: API keys changed => rebuilding provider list.");

        m_weatherProviders = WeatherProviderFactory::createAll(*m_networkClient, *m_settings);

        if (!m_currentWeatherProvider || m_weatherProviders.find(m_currentWeatherProvider->id()) == m_weatherProviders.end()) {
            rebuildWeatherProvider();
            if (m_weatherRepository && m_currentWeatherProvider) {
                m_weatherRepository->setProvider(m_currentWeatherProvider);
            }
        }

        emit providersChanged();
    });

    connect(translation().get(), &TranslationManager::languageChanged, this, [this]() {
        switch (m_settings->locale()) {
            case Locale::English: QLocale::setDefault(QLocale(QLocale::English)); break;
            case Locale::Ukrainian: QLocale::setDefault(QLocale(QLocale::Ukrainian)); break;
            default: QLocale::setDefault(QLocale(QLocale::system())); break;
        }
    });
}
