#ifndef INFRASTRUCTURE_THEME_MANAGER_H
#define INFRASTRUCTURE_THEME_MANAGER_H

#include <QPalette>
#include <QObject>

class SettingsManager;

class ThemeManager final : public QObject {
    Q_OBJECT
public:
    explicit ThemeManager(SettingsManager *settings, QObject *parent = nullptr);
    void applyTheme();

private:
    SettingsManager *m_settings;
    void setDarkTheme();
    void setLightTheme();
    void setAutoTheme();
};

#endif //INFRASTRUCTURE_THEME_MANAGER_H
