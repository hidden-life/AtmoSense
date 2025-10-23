#ifndef DOMAIN_MODEL_THEME_H
#define DOMAIN_MODEL_THEME_H

#include <QString>
#include <QMetaType>

enum class Theme {
    System = 0,
    Auto,
    Dark,
    Light,
};

namespace ThemeUtils {
    // for settings naming
    inline QString toString(const Theme theme) {
        switch (theme) {
            case Theme::Auto:
                return "auto";
            case Theme::Dark:
                return "dark";
            case Theme::Light:
                return "light";
            default:
                return "system";
        }
    }

    // from settings naming
    inline Theme fromString(const QString &str) {
        const QString trimmed = str.toLower().trimmed();
        if (trimmed == "light") return Theme::Light;
        if (trimmed == "dark") return Theme::Dark;
        if (trimmed == "system") return Theme::System;

        return Theme::Auto;
    }

    // for UI display
    inline QString toDisplayName(const Theme theme) {
        switch (theme) {
            case Theme::Auto: return "Auto";
            case Theme::Dark: return "Dark";
            case Theme::Light: return "Light";
            default: return "System";
        }
    }

    // from UI
    inline Theme fromDisplayName(const QString &name) {
        const QString trimmed = name.toLower().trimmed();
        if (trimmed == "dark") return Theme::Dark;
        if (trimmed == "light") return Theme::Light;
        if (trimmed == "system") return Theme::System;

        return Theme::Auto;
    }

    inline bool isDark(const Theme theme) {
        return theme == Theme::Dark;
    }

    inline bool isLight(const Theme theme) {
        return theme == Theme::Light;
    }

    inline bool isAuto(const Theme theme) {
        return theme == Theme::Auto;
    }

    inline bool isSystem(const Theme theme) {
        return theme == Theme::System;
    }
}

Q_DECLARE_METATYPE(Theme)

#endif //DOMAIN_MODEL_THEME_H
