#include <QTimer>
#include <QLocale>

#include "Application.h"

#include "IconMapper.h"
#include "SettingsDialog.h"
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

    m_mainWindow = std::make_unique<MainWindow>();
    m_trayService = std::make_unique<TrayService>(m_ctx.get());
}

Application::~Application() = default;

int Application::start() {
    Logger::info("Application started.");

    auto &settings = *m_ctx->settings();
    QLocale::setDefault(QLocale(LanguageUtils::toCode(settings.language())));

    // start location, by default Kyiv
    const double lat = settings.latitude();
    const double lon = settings.longitude();
    const QString tz = "auto";

    // receive weather and update UI
    auto &repository = *m_ctx->weatherRepository();
    GetWeatherUseCase weatherUseCase(repository);

    auto refresh = [this, &weatherUseCase, lat, lon, tz]() {
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

    int minutes = settings.refreshInterval();
    auto *timer = new QTimer(&m_app);
    QObject::connect(timer, &QTimer::timeout, &m_app, refresh);
    timer->start(minutes * 60 * 1000);

    // tray
    QObject::connect(m_trayService.get(), &TrayService::openAction, [&]() {
        m_mainWindow->show();
        m_mainWindow->raise();
        m_mainWindow->activateWindow();
    });

    QObject::connect(m_trayService.get(), &TrayService::refreshAction, refresh);

    QObject::connect(m_ctx->updateScheduler().get(), &UpdateScheduler::update, [this]() {
        Logger::info("Scheduled update triggered -> refreshing weather...");
        // here we need to call repository for weather update
    });

    auto tr = m_ctx->translation();
    QObject::connect(tr.get(), &TranslationManager::languageChanged, m_mainWindow.get(), &MainWindow::retranslate);
    QObject::connect(tr.get(), &TranslationManager::languageChanged, m_trayService.get(), &TrayService::retranslate);

    QObject::connect(m_trayService.get(), &TrayService::openSettingsRequested, this, &Application::showSettings);

    // show window (comment if you don't want to show it after start)
    m_mainWindow->show();
    Logger::info("Main window shown.");

    return m_app.exec();
}

void Application::showSettings() {
    SettingsDialog settingsDialog(m_ctx->settings(), m_mainWindow.get());
    settingsDialog.exec();
}