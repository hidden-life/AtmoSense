#include "NotificationSoundPlayer.h"
#include "Logger.h"

NotificationSoundPlayer::NotificationSoundPlayer(QObject *parent) {
    auto createSound = [&](NotificationType type, const QString &file) {
        auto *sound = new QSoundEffect(this);
        sound->setSource(QUrl::fromLocalFile(file));
        sound->setVolume(0.8);
        m_sounds[type] = sound;
    };

    createSound(NotificationType::Rain, "qrc:/sounds/notification.wav");
    createSound(NotificationType::Wind, "qrc:/sounds/notification.wav");
    createSound(NotificationType::Freeze, "qrc:/sounds/notification.wav");
}

void NotificationSoundPlayer::play(NotificationType type) {
    if (!m_sounds.contains(type)) {
        Logger::warn("NotificationSoundPlayer: unknown sound type.");
        return;
    }

    auto *sound = m_sounds[type];
    if (!sound->isLoaded()) {
        Logger::warn("NotificationSoundPlayer: sound not loaded, skipping.");
        return;
    }

    sound->play();
}

void NotificationSoundPlayer::setVolume(const qreal volume) {
    for (auto *sound : m_sounds) {
        sound->setVolume(volume);
    }
}
