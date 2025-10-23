#ifndef INFRASTRUCTURE_THEME_MANAGER_H
#define INFRASTRUCTURE_THEME_MANAGER_H

#include <QPalette>
#include <QObject>

#include "model/Theme.h"

class SettingsManager;

class ThemeManager final : public QObject {
    Q_OBJECT
public:
    explicit ThemeManager(std::shared_ptr<SettingsManager> settings, QObject *parent = nullptr);
    void applyTheme();

    Theme theme() const { return m_theme; }
    void setTheme(Theme theme);

    bool isDarkTheme() const;
    bool isLightTheme() const;

signals:
    void themeChanged(Theme theme);

private:
    std::shared_ptr<SettingsManager> m_settings;
    Theme m_theme;

    void setDarkTheme();
    void setLightTheme();
    void setAutoTheme();
    void applyPalette(Theme theme);
};

#endif //INFRASTRUCTURE_THEME_MANAGER_H
