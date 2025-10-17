#include <QApplication>
#include <QResource>

#include "TranslationManager.h"

#include <QDir>

#include "Logger.h"
#include "SettingsManager.h"
#include "model/Language.h"

TranslationManager::TranslationManager(std::shared_ptr<SettingsManager> settings, QObject *parent) : QObject(parent), m_settings(std::move(settings)) {
    connect(m_settings.get(), &SettingsManager::settingsChanged, this, [this] {
        apply();
    });
}

TranslationManager::~TranslationManager() = default;

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
    const QString path = QString(":/translations/atmo_sense_%1.qm").arg(code);

    auto tr = std::make_unique<QTranslator>();
    if (const bool isOk = tr->load(path); !isOk) {
        Logger::warn("Failed to load translations for " + path + ", falling back to English.");
        if (code != "en") {
            if (const bool ok = tr->load(":/translations/atmo_sense_en.qm"); !ok) {
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

