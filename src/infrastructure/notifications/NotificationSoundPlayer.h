#ifndef INFRASTRUCTURE_NOTIFICATIONS_NOTIFICATION_SOUND_PLAYER_H
#define INFRASTRUCTURE_NOTIFICATIONS_NOTIFICATION_SOUND_PLAYER_H

#include <QObject>
#include <QtMultimedia/QSoundEffect>
#include <QMap>

#include "model/Notification.h"

class NotificationSoundPlayer final : public QObject {
    Q_OBJECT
public:
    explicit NotificationSoundPlayer(QObject *parent = nullptr);

    void play(NotificationType type);
    void setVolume(qreal volume); // 0.0 - 1.0

private:
    QMap<NotificationType, QSoundEffect*> m_sounds;
};

#endif //INFRASTRUCTURE_NOTIFICATIONS_NOTIFICATION_SOUND_PLAYER_H
