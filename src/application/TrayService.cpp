#include <QAction>

#include "TrayService.h"

#include "Logger.h"
#include "MainWindow.h"
#include "SettingsDialog.h"

TrayService::TrayService(ApplicationContext *ctx, QObject *parent) : QObject(parent), m_ctx(ctx) {
    m_openAction = new QAction(tr("Open"), this);
    m_refreshAction = new QAction(tr("Refresh"), this);
    m_quitAction = new QAction(tr("Quit"), this);
    m_settingsAction = new QAction(tr("Settings"), this);

    m_menu.addAction(m_openAction);
    m_menu.addAction(m_refreshAction);
    m_menu.addSeparator();
    m_menu.addAction(m_settingsAction);
    m_menu.addSeparator();
    m_menu.addAction(m_quitAction);

    connect(m_openAction, &QAction::triggered, this, &TrayService::openAction);
    connect(m_refreshAction, &QAction::triggered, this, &TrayService::refreshAction);
    connect(m_settingsAction, &QAction::triggered, this, [this]() {
        emit openSettingsRequested();
    });
    connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    m_tray.setToolTip(tr("AtmoSense: fast weather info"));
    m_tray.setContextMenu(&m_menu);
    const QIcon defaultIcon(":/icons/weather/day/clear.png"); // default icon, may be need to change in future
    if (defaultIcon.isNull()) {
        Logger::warning("Default icon not found!");
    }
    m_tray.setIcon(defaultIcon);
    m_tray.show();
}

void TrayService::setIcon(const QIcon &icon) {
    m_tray.setIcon(icon);
}

void TrayService::showInfo(const QString &title, const QString &message) {
    m_tray.showMessage(title, message, QSystemTrayIcon::Information, 3000);
}

void TrayService::retranslate() {
    m_openAction->setText(tr("Open"));
    m_refreshAction->setText(tr("Refresh"));
    m_quitAction->setText(tr("Quit"));
    m_settingsAction->setText(tr("Settings"));

    m_tray.setToolTip(tr("AtmoSense: fast weather info"));
    m_tray.setVisible(true);
}

