#include <QApplication>
#include <QStyleFactory>
#include <QDateTime>

#include "SettingsManager.h"
#include "ThemeManager.h"
#include "model/Theme.h"

ThemeManager::ThemeManager(SettingsManager *settings, QObject *parent) : QObject(parent), m_settings(settings) {
    connect(m_settings, &SettingsManager::settingsChanged, this, &ThemeManager::applyTheme);
    applyTheme();
}

void ThemeManager::applyTheme() {
    Theme theme = m_settings->theme();
    switch (theme) {
        case Theme::Light:
            setLightTheme();
            break;
        case Theme::Dark:
            setDarkTheme();
            break;
        case Theme::Auto:
        default:
            setAutoTheme();
            break;
    }
}

void ThemeManager::setLightTheme() {
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(250, 250, 250));
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, QColor(245, 245, 245));
    palette.setColor(QPalette::Text, Qt::black);
    qApp->setPalette(palette);
}

void ThemeManager::setDarkTheme() {
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(35, 35, 35));
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
    qApp->setPalette(palette);
}

void ThemeManager::setAutoTheme() {
#ifdef Q_OS_WIN
    if (const int hour = QTime::currentTime().hour(); hour >= 7 && hour <= 20) {
        setLightTheme();
    } else {
        setDarkTheme();
    }
#else
    setLightTheme();
#endif
}