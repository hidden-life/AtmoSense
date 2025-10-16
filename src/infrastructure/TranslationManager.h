#ifndef INFRASTRUCTURE_TRANSLATION_MANAGER_H
#define INFRASTRUCTURE_TRANSLATION_MANAGER_H

#include <QCoreApplication>
#include <QObject>

class SettingsManager;

class TranslationManager final : public QObject {
    Q_OBJECT
public:
    explicit TranslationManager(SettingsManager *settings, QObject *parent = nullptr);
    void apply();

signals:
    void languageChanged();

private:
    SettingsManager *m_settings;
    std::unique_ptr<QTranslator> m_translator;

    void install(const QString &code);
};

#endif //INFRASTRUCTURE_TRANSLATION_MANAGER_H
