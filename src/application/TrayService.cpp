#include <QAction>

#include "TrayService.h"
#include "MainWindow.h"

TrayService::TrayService(QObject *parent) : QObject(parent) {
    auto *open = new QAction("Open", &m_menu);
    auto *refresh = new QAction("Refresh", &m_menu);
    auto *quit = new QAction("Quit", &m_menu);

    m_menu.addAction(open);
    m_menu.addAction(refresh);
    m_menu.addSeparator();
    m_menu.addAction(quit);

    QObject::connect(open, &QAction::triggered, this, &TrayService::openAction);
    QObject::connect(refresh, &QAction::triggered, this, &TrayService::refreshAction);
    QObject::connect(quit, &QAction::triggered, qApp, &QCoreApplication::quit);

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

