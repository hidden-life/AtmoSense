#ifndef APPLICATION_TRAY_SERVICE_H
#define APPLICATION_TRAY_SERVICE_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCoreApplication>

#include "core/ApplicationContext.h"

class TrayService final : public QObject {
    Q_OBJECT
public:
    explicit TrayService(ApplicationContext *ctx, QObject *parent = nullptr);
    void setIcon(const QIcon &icon);
    void showInfo(const QString &title, const QString &message);

public slots:
    void retranslate();

signals:
    void openAction();
    void refreshAction();
    void openSettingsRequested();

private:
    QSystemTrayIcon m_tray;
    QMenu m_menu;
    ApplicationContext *m_ctx;

    QAction *m_openAction = nullptr;
    QAction *m_refreshAction = nullptr;
    QAction *m_quitAction = nullptr;
    QAction *m_settingsAction = nullptr;
};

#endif //APPLICATION_TRAY_SERVICE_H
