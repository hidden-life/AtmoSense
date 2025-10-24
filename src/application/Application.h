#ifndef APPLICATION_APPLICATION_H
#define APPLICATION_APPLICATION_H

#include <QApplication>
#include <memory>

#include "AlertService.h"

class MainWindow;
class ApplicationContext;
class TrayService;

class Application : public QObject {
    Q_OBJECT
    QApplication m_app;
    std::unique_ptr<ApplicationContext> m_ctx;
    std::unique_ptr<MainWindow> m_mainWindow;
    std::unique_ptr<TrayService> m_trayService;
    std::unique_ptr<AlertService> m_alertService;

public slots:
    void showSettings();

private slots:
    void fetchWeather();

public:
    Application(int &argc, char **argv);
    ~Application() override;
    int start();
};

#endif //APPLICATION_APPLICATION_H
