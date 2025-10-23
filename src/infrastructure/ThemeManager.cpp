#include <QApplication>
#include <QStyleFactory>
#include <QDateTime>
#include <QStyle>

#include "SettingsManager.h"
#include "ThemeManager.h"
#include "model/Theme.h"

ThemeManager::ThemeManager(std::shared_ptr<SettingsManager> settings, QObject *parent) : QObject(parent), m_settings(std::move(settings)) {
    m_theme = m_settings->theme();
    applyPalette(m_theme);
}

void ThemeManager::setLightTheme() {
    QPalette palette = QPalette();
    qApp->setStyle(QStyleFactory::create("Fusion"));
    palette = QPalette();
    palette.setColor(QPalette::Window, QColor(240, 240, 240));
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, Qt::white);
    palette.setColor(QPalette::AlternateBase, QColor(233, 233, 233));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Button, QColor(240, 240, 240));
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, QColor(38, 79, 120));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    qApp->setPalette(palette);
}

void ThemeManager::setDarkTheme() {
    QPalette palette = QPalette();
    qApp->setStyle(QStyleFactory::create("Fusion"));
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(42, 42, 42));
    palette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    palette.setColor(QPalette::ToolTipBase, QColor(53, 53, 53));
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
    palette.setColor(QPalette::HighlightedText, Qt::black);
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

void ThemeManager::applyPalette(const Theme theme) {
    QPalette palette;
    switch (theme) {
        case Theme::System:
            qApp->setStyle(QStyleFactory::create("Fusion"));
            qApp->setPalette(QApplication::style()->standardPalette());
            break;
        case Theme::Auto:
            setAutoTheme();
            break;
        case Theme::Light:
            setLightTheme();
            break;
        case Theme::Dark:
            setDarkTheme();
            break;
    }
}

void ThemeManager::setTheme(const Theme theme) {
    if (m_theme == theme) {
        return;
    }

    m_theme = theme;
    m_settings->setTheme(theme);
    applyPalette(theme);

    emit themeChanged(theme);
}

bool ThemeManager::isDarkTheme() const {
    return m_theme == Theme::Dark;
}

bool ThemeManager::isLightTheme() const {
    return m_theme == Theme::Light;
}
