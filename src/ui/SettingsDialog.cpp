#include "SettingsDialog.h"
#include "SettingsManager.h"
#include "./ui_settingsdialog.h"
#include "ApplicationContext.h"

SettingsDialog::SettingsDialog(std::shared_ptr<SettingsManager> settings, ApplicationContext *ctx, QWidget *parent) :
    QDialog(parent), ui(new Ui::SettingsDialog), m_settings(std::move(settings)), m_ctx(ctx) {
    ui->setupUi(this);

    populateProviders();

    // fill themes
    ui->themeComboBox->clear();
    ui->themeComboBox->addItem(ThemeUtils::toDisplayName(Theme::Auto));
    ui->themeComboBox->addItem(ThemeUtils::toDisplayName(Theme::Light));
    ui->themeComboBox->addItem(ThemeUtils::toDisplayName(Theme::Dark));

    // set current theme from settings
    const Theme currentTheme = m_settings->theme();
    ui->themeComboBox->setCurrentText(ThemeUtils::toDisplayName(currentTheme));

    ui->tempUnitComboBox->setCurrentText(m_settings->temperatureUnit());
    ui->windSpeedUnitComboBox->setCurrentText(m_settings->windSpeedUnit());
    ui->providerComboBox->setCurrentText(m_settings->provider());
    ui->refreshIntervalSpinBox->setValue(m_settings->refreshInterval());

    // language
    ui->languageComboBox->clear();
    ui->languageComboBox->addItem(LanguageUtils::toDisplayName(Language::English), LanguageUtils::toCode(Language::English));
    ui->languageComboBox->addItem(LanguageUtils::toDisplayName(Language::Ukrainian), LanguageUtils::toCode(Language::Ukrainian));

    const QString currentCode = LanguageUtils::toCode(m_settings->language());
    if (const int idx = ui->languageComboBox->currentIndex(); idx >= 0) {
        ui->languageComboBox->setCurrentIndex(idx);
    }

    const QString selectedCode = ui->languageComboBox->currentData().toString();
    m_settings->setLanguage(LanguageUtils::fromCode(selectedCode));

    ui->hourlyDisplaySpinBox->setValue(m_settings->hourlyDisplayHours());

    connect(ui->saveButton, &QPushButton::clicked, this, &SettingsDialog::onSaveButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &SettingsDialog::onCancelButtonClicked);
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::retranslate() {
    ui->retranslateUi(this);
}

void SettingsDialog::populateProviders() {
    //clear before fill
    ui->providerComboBox->clear();
    ui->geocoderComboBox->clear();

    // add available weather providers
    for (const auto &pair : m_ctx->weatherProviders()) {
        ui->providerComboBox->addItem(pair.first);
    }

    // add available geocoder providers
    for (const auto &pair : m_ctx->geocoderProviders()) {
        ui->geocoderComboBox->addItem(pair.first);
    }

    // enable/disable based on count
    ui->providerComboBox->setEnabled(m_ctx->weatherProviderCount() > 1);
    ui->geocoderComboBox->setEnabled(m_ctx->geocoderProviderCount() > 1);

    if (!m_ctx->weatherProviders().empty()) ui->providerComboBox->setCurrentIndex(0);
    if (!m_ctx->geocoderProviders().empty()) ui->geocoderComboBox->setCurrentIndex(0);

    // set an active as default
    if (auto activeWeatherProvider = m_ctx->currentWeatherProvider()) {
        const QString key = std::find_if(
                m_ctx->weatherProviders().begin(), m_ctx->weatherProviders().end(),
                [&](auto &w) { return w.second == activeWeatherProvider; }
            )->first;
        ui->providerComboBox->setCurrentText(key);
    }

    if (auto activeGeocoderProvider = m_ctx->currentGeocoderProvider()) {
        const QString key = std::find_if(
                m_ctx->geocoderProviders().begin(), m_ctx->geocoderProviders().end(),
                [&](auto &g) { return g.second == activeGeocoderProvider; }
        )->first;
    }
}

void SettingsDialog::onSaveButtonClicked() {
    // save theme
    const Theme selectedTheme = ThemeUtils::fromDisplayName(ui->themeComboBox->currentText());
    m_settings->setTheme(selectedTheme);

    m_settings->setTemperatureUnit(ui->tempUnitComboBox->currentText());
    m_settings->setWindSpeedUnit(ui->windSpeedUnitComboBox->currentText());
    m_settings->setProvider(ui->providerComboBox->currentText());
    m_settings->setRefreshInterval(ui->refreshIntervalSpinBox->value());
    m_settings->setLanguage(LanguageUtils::fromCode(ui->languageComboBox->currentData().toString()));
    m_settings->setHourlyDisplayHours(ui->hourlyDisplaySpinBox->value());

    accept();
}

void SettingsDialog::onCancelButtonClicked() {
    reject();
}
