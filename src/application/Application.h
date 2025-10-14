#ifndef APPLICATION_APPLICATION_H
#define APPLICATION_APPLICATION_H

#include <QApplication>
#include <memory>

class MainWindow;
class ApplicationContext;
class TrayService;

class Application {
    QApplication m_app;
    std::unique_ptr<ApplicationContext> m_ctx;
    std::unique_ptr<MainWindow> m_mainWindow;
    std::unique_ptr<TrayService> m_trayService;

public:
    Application(int &argc, char **argv);
    ~Application();
    int start();
};

#endif //APPLICATION_APPLICATION_H
