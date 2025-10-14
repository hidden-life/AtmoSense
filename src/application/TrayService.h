#ifndef APPLICATION_TRAY_SERVICE_H
#define APPLICATION_TRAY_SERVICE_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCoreApplication>

class TrayService : public QObject {
    Q_OBJECT
public:
    explicit TrayService(QObject *parent = nullptr);
    void setIcon(const QIcon &icon);
    void showInfo(const QString &title, const QString &message);

signals:
    void openAction();
    void refreshAction();

private:
    QSystemTrayIcon m_tray;
    QMenu m_menu;
};

#endif //APPLICATION_TRAY_SERVICE_H
