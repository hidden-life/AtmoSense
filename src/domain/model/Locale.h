#ifndef DOMAIN_MODEL_LOCALE_H
#define DOMAIN_MODEL_LOCALE_H

#include <QString>

enum class Locale {
    System,
    English,
    Ukrainian,
};

inline QString toString(const Locale locale) {
    switch (locale) {
        case Locale::English: return "en";
        case Locale::Ukrainian: return "uk";
        default: return "system";
    }
}

inline Locale toLocale(const QString &str) {
    if (str == "en") return Locale::English;
    if (str == "uk") return Locale::Ukrainian;
    return Locale::System;
}

#endif //DOMAIN_MODEL_LOCALE_H
