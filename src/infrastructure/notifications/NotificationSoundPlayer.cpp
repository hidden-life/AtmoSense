#include <QDir>

#include "NotificationSoundPlayer.h"
#include "Logger.h"

NotificationSoundPlayer::NotificationSoundPlayer(QObject *parent) {
    auto initSound = [&](const NotificationType type, const QString &file) {
        auto *sound = new QSoundEffect(this);
        sound->setSource(QUrl(file));
        sound->setVolume(0.8);
        m_sounds[type] = sound;
    };

    initSound(NotificationType::Rain, "qrc:/sounds/notification.wav");
    initSound(NotificationType::Wind, "qrc:/sounds/notification.wav");
    initSound(NotificationType::Freeze, "qrc:/sounds/notification.wav");
}

void NotificationSoundPlayer::play(NotificationType type) {
    if (!m_sounds.contains(type)) {
        Logger::warning("NotificationSoundPlayer: unknown sound type.");
        return;
    }

    auto *sound = m_sounds[type];
    if (!sound->isLoaded()) {
        Logger::warning("NotificationSoundPlayer: sound not loaded, skipping.");
        return;
    }

    sound->play();
}

void NotificationSoundPlayer::setVolume(const qreal volume) {
    for (auto *sound : m_sounds) {
        sound->setVolume(volume);
    }
}
