#ifndef APPLICATION_UPDATE_SCHEDULER_H
#define APPLICATION_UPDATE_SCHEDULER_H

#include <QObject>
#include <QTimer>

class SettingsManager;

class UpdateScheduler final : public QObject {
    Q_OBJECT
public:
    explicit UpdateScheduler(std::shared_ptr<SettingsManager> settings, QObject *parent = nullptr);

    void pause();
    void resume();

signals:
    void update();

private slots:
    void onSettingsChanged();
    void onTimeout();

private:
    std::shared_ptr<SettingsManager> m_settings;
    QTimer m_timer;
    qint64 m_lastTrigger = 0;

    void restartTimer();
};

#endif //APPLICATION_UPDATE_SCHEDULER_H
