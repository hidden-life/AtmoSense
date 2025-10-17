#ifndef INFRASTRUCTURE_TRANSLATION_MANAGER_H
#define INFRASTRUCTURE_TRANSLATION_MANAGER_H

#include <QCoreApplication>
#include <QObject>
#include <QTranslator>

class SettingsManager;

class TranslationManager final : public QObject {
    Q_OBJECT
public:
    explicit TranslationManager(std::shared_ptr<SettingsManager> settings, QObject *parent = nullptr);
    ~TranslationManager();
    void apply();

signals:
    void languageChanged();

private:
    std::shared_ptr<SettingsManager> m_settings;
    std::unique_ptr<QTranslator> m_translator;

    void install(const QString &code);
};

#endif //INFRASTRUCTURE_TRANSLATION_MANAGER_H
