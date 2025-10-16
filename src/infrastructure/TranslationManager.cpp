#include <QApplication>
#include  <QTranslator>

#include "TranslationManager.h"

#include "Logger.h"
#include "SettingsManager.h"
#include "model/Language.h"

TranslationManager::TranslationManager(SettingsManager *settings, QObject *parent) : QObject(parent), m_settings(settings) {
    connect(m_settings, &SettingsManager::settingsChanged, this, [this] {
        apply();
    });
}

void TranslationManager::apply() {
    const QString code = LanguageUtils::toCode(m_settings->language());
    install(code);

    emit languageChanged();
}

void TranslationManager::install(const QString &code) {
    if (m_translator) {
        qApp->removeTranslator(m_translator.get());
        m_translator.reset();
    }

    // path to translations
    const QString path = QString(":/translations/atmo_sens_%1.qm").arg(code);

    auto tr =std::make_unique<QTranslator>();
    const bool isOk = tr->load(path);
    if (!isOk) {
        Logger::warn("Failed to load translations for " + path + ", falling back to English.");
        if (code != "en") {
            const bool ok = tr->load(":/translations/atmo_sens_en.qm");
            if (!ok) {
                Logger::warn("Fallback EN translator not found, continuing without translator.");
                return;
            }
        } else {
            return;
        }
    }

    qApp->installTranslator(tr.get());
    m_translator = std::move(tr);
}

