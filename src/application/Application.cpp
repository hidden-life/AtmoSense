#include <QTimer>
#include <QLocale>

#include "Application.h"

#include "IconMapper.h"
#include "SettingsDialog.h"
#include "ThemeManager.h"
#include "TranslationManager.h"
#include "UpdateScheduler.h"
#include "ui/MainWindow.h"
#include "infrastructure/SettingsManager.h"
#include "infrastructure/Logger.h"
#include "core/ApplicationContext.h"
#include "application/TrayService.h"
#include "application/IconMapper.h"
#include "use_case/GetWeatherUseCase.h"

Application::Application(int &argc, char **argv) : m_app(argc, argv) {
    m_app.setApplicationName("AtmoSense");
    m_app.setOrganizationName("AtmoSense Labs");
    m_app.setQuitOnLastWindowClosed(false);
    m_ctx = std::make_unique<ApplicationContext>();
    m_ctx->init();

    Logger::init();

    m_mainWindow = std::make_unique<MainWindow>(m_ctx.get());
    m_mainWindow->restoreLastLocation();
    m_trayService = std::make_unique<TrayService>(m_ctx.get());
    m_alertService = std::make_unique<AlertService>(m_ctx->settings().get(), m_trayService.get());
}

Application::~Application() {
    Logger::shutdown();
}

int Application::start() {
    Logger::info("Application started.");
    QFile qss(":/styles/default.qss");
    if (qss.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(qss.readAll());
    }

    const auto &settings = *m_ctx->settings();
    QLocale::setDefault(QLocale(LanguageUtils::toCode(settings.language())));

    // start location, by default Kyiv
    const double lat = settings.latitude();
    const double lon = settings.longitude();
    const QString tz = "auto";

    // receive weather and update UI
    auto &repository = *m_ctx->weatherRepository();
    GetWeatherUseCase weatherUseCase(repository);

    auto refresh = [this, &weatherUseCase]() {
        const auto settings = m_ctx->settings();
        const double lat = settings->latitude();
        const double lon = settings->longitude();
        const QString tz = "auto";

        try {
            const auto f = weatherUseCase(lat, lon, tz, 10);
            m_mainWindow->displayForecast(f, QString("lat=%1 lon=%2").arg(lat).arg(lon));
            m_trayService->setIcon(IconMapper::iconFor(f.weather.weatherCode, false));
            m_trayService->showInfo("Weather updated", "Forecast refreshed");
        } catch (const std::exception &e) {
            Logger::error(QString("Refresh failed: %1").arg(e.what()));
            m_trayService->showInfo("Weather error", e.what());
        }
    };

    refresh();

    const int minutes = settings.refreshInterval();
    auto *timer = new QTimer(&m_app);
    connect(timer, &QTimer::timeout, &m_app, refresh);
    timer->start(minutes * 60 * 1000);

    // tray
    connect(m_trayService.get(), &TrayService::openAction, [&]() {
        m_mainWindow->show();
        m_mainWindow->raise();
        m_mainWindow->activateWindow();
    });

    connect(m_trayService.get(), &TrayService::refreshAction, refresh);

    connect(m_ctx->updateScheduler().get(), &UpdateScheduler::update, [this]() {
        Logger::info("Scheduled update triggered -> refreshing weather...");
        // here we need to call repository for weather update
    });

    auto tr = m_ctx->translation();
    connect(tr.get(), &TranslationManager::languageChanged, m_mainWindow.get(), &MainWindow::retranslate);
    connect(tr.get(), &TranslationManager::languageChanged, m_trayService.get(), &TrayService::retranslate);

    connect(m_trayService.get(), &TrayService::openSettingsRequested, this, &Application::showSettings);

    connect(m_ctx->updateScheduler().get(), &UpdateScheduler::update, this, &Application::fetchWeather);
    QMetaObject::invokeMethod(this, &Application::fetchWeather, Qt::QueuedConnection);

    connect(m_mainWindow.get(), &MainWindow::refreshRequested, this, &Application::fetchWeather);
    connect(m_mainWindow.get(), &MainWindow::openSettingsRequested, this, &Application::showSettings);

    // show window (comment if you don't want to show it after start)
    m_mainWindow->show();
    Logger::info("Main window shown.");

    return m_app.exec();
}

void Application::showSettings() {
    auto tr = m_ctx->translation();
    tr->blockSignals(true);
    SettingsDialog settingsDialog(m_ctx.get(), m_mainWindow.get());
    settingsDialog.exec();
    tr->blockSignals(false);
}

void Application::fetchWeather() {
    Logger::info("Application: Fetching weather...");

    auto &repository = *m_ctx->weatherRepository();
    const auto settings = m_ctx->settings();

    const double lat = settings->latitude();
    const double lon = settings->longitude();
    const QString tz = "auto"; // change in the future, by adding new method to SettingsManager: timezone()

    try {
        const auto forecast = repository.get(lat, lon ,tz, 60);
        Logger::info("Application: weather data fetched successfully.");

        // update UI
        if (m_mainWindow) {
            m_mainWindow->updateWeather(forecast);
        }

        // update tray icon
        if (m_trayService) {
            const bool isDark = m_ctx->themeManager()->isDarkTheme();
            const auto icon = IconMapper::map(forecast.weather.weatherCode, isDark);
            m_trayService->setIcon(icon);
        }

        // clever alerts
        if (m_alertService) {
            m_alertService->process(forecast);
        }
    } catch (const std::exception &e) {
        Logger::error("Failed to fetch weather: " + QString::fromStdString(e.what()));
    }
}
