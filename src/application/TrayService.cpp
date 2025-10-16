#include <QAction>

#include "TrayService.h"
#include "MainWindow.h"
#include "SettingsDialog.h"

TrayService::TrayService(ApplicationContext *ctx, QObject *parent) : QObject(parent), m_ctx(ctx) {
    auto *open = new QAction("Open", &m_menu);
    auto *refresh = new QAction("Refresh", &m_menu);
    auto *quit = new QAction("Quit", &m_menu);
    auto *settings = new QAction("Settings", &m_menu);

    m_menu.addAction(open);
    m_menu.addAction(refresh);
    m_menu.addAction(settings);
    m_menu.addSeparator();
    m_menu.addAction(quit);

    connect(open, &QAction::triggered, this, &TrayService::openAction);
    connect(refresh, &QAction::triggered, this, &TrayService::refreshAction);
    connect(settings, &QAction::triggered, [this]() {
        const auto settingsCtx = m_ctx->settings();
        SettingsDialog settingsDialog(settingsCtx.get());
        settingsDialog.exec();
    });
    connect(quit, &QAction::triggered, qApp, &QCoreApplication::quit);

    m_tray.setToolTip("AtmoSense: fast weather info");
    m_tray.setContextMenu(&m_menu);
    m_tray.show();
}

void TrayService::setIcon(const QIcon &icon) {
    m_tray.setIcon(icon);
}

void TrayService::showInfo(const QString &title, const QString &message) {
    m_tray.showMessage(title, message, QSystemTrayIcon::Information, 3000);
}

