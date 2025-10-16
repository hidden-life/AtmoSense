#ifndef APPLICATION_TRAY_SERVICE_H
#define APPLICATION_TRAY_SERVICE_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCoreApplication>

#include "core/ApplicationContext.h"

class TrayService : public QObject {
    Q_OBJECT
public:
    explicit TrayService(ApplicationContext *ctx, QObject *parent = nullptr);
    void setIcon(const QIcon &icon);
    void showInfo(const QString &title, const QString &message);

signals:
    void openAction();
    void refreshAction();

private:
    QSystemTrayIcon m_tray;
    QMenu m_menu;
    ApplicationContext *m_ctx;
};

#endif //APPLICATION_TRAY_SERVICE_H
