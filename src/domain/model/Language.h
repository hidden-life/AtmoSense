#ifndef DOMAIN_MODEL_LANGUAGE_H
#define DOMAIN_MODEL_LANGUAGE_H

#include <QString>
#include <QMetaType>

enum class Language {
    English,
    Ukrainian,
};

namespace LanguageUtils {
    inline QString toCode(Language lang) {
        switch (lang) {
            case Language::English: return "en";
            case Language::Ukrainian: return "uk";
        }

        return "en";
    }

    inline Language fromCode(const QString &code) {
        if (const QString str = code.toLower().trimmed(); str == "uk" || str == "uk_UA") return Language::Ukrainian;

        return Language::English;
    }

    inline QString toDisplayName(Language lang) {
        switch (lang) {
            case Language::English: return "English";
            case Language::Ukrainian: return "Українська";
        }

        return "English";
    }
}

Q_DECLARE_METATYPE(Language)
#endif //DOMAIN_MODEL_LANGUAGE_H
