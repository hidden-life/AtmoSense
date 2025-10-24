#ifndef DOMAIN_MODEL_NOTIFICATION_H
#define DOMAIN_MODEL_NOTIFICATION_H

#include <QString>

enum class NotificationType {
    Rain = 1,
    Wind,
    Freeze,
};

inline QString toString(const NotificationType type) {
    switch (type) {
        case NotificationType::Rain: return "rain";
        case NotificationType::Wind: return "wind";
        case NotificationType::Freeze: return "freeze";
        default:
            break;
    }

    return "unknown";
}

inline NotificationType toNotificationType(const QString &str) {
    if (str == "rain") return NotificationType::Rain;
    if (str == "wind") return NotificationType::Wind;
    if (str == "freeze") return NotificationType::Freeze;

    return NotificationType::Rain;
}

#endif //DOMAIN_MODEL_NOTIFICATION_H
