#include "SettingsDialog.h"
#include "SettingsManager.h"
#include "./ui_settingsdialog.h"
#include "ApplicationContext.h"
#include "ThemeManager.h"
#include "model/Provider.h"

SettingsDialog::SettingsDialog(ApplicationContext *ctx, QWidget *parent) :
    QDialog(parent), ui(new Ui::SettingsDialog), m_ctx(ctx) {
    ui->setupUi(this);

    const auto settings = m_ctx->settings();
    // language
    ui->languageComboBox->clear();
    ui->languageComboBox->addItem(LanguageUtils::toDisplayName(Language::English), LanguageUtils::toCode(Language::English));
    ui->languageComboBox->addItem(LanguageUtils::toDisplayName(Language::Ukrainian), LanguageUtils::toCode(Language::Ukrainian));
    {
        const QString code = LanguageUtils::toCode(settings->language());
        int idx = ui->languageComboBox->findData(code);
        if (idx < 0) {
            idx = 0;
        }
        ui->languageComboBox->setCurrentIndex(idx);
    }

    populateProviders();

    // fill themes
    ui->themeComboBox->clear();
    ui->themeComboBox->addItem(ThemeUtils::toDisplayName(Theme::Auto));
    ui->themeComboBox->addItem(ThemeUtils::toDisplayName(Theme::Light));
    ui->themeComboBox->addItem(ThemeUtils::toDisplayName(Theme::Dark));
    {
        const int val = static_cast<int>(settings->theme());
        int idx = ui->themeComboBox->findData(val);
        if (idx < 0) {
            idx = 0;
        }
        ui->themeComboBox->setCurrentIndex(idx);
    }

    // // set current theme from settings
    // const Theme currentTheme = m_settings->theme();
    // ui->themeComboBox->setCurrentText(ThemeUtils::toDisplayName(currentTheme));

    ui->providerComboBox->setCurrentText(settings->provider());
    ui->refreshIntervalSpinBox->setValue(settings->refreshInterval());

    // units
    ui->unitsComboBox->clear();
    ui->unitsComboBox->addItem(tr("Metric (°C, m/s, hPa)"), static_cast<int>(UnitSystem::Metric));
    ui->unitsComboBox->addItem(tr("Imperial (°F, mph, inHg)"), static_cast<int>(UnitSystem::Imperial));
    {
        const UnitSystem s = settings->unitSystem();
        int idx = ui->unitsComboBox->findData(static_cast<int>(s));
        if (idx < 0) {
            idx = 0;
        }
        ui->unitsComboBox->setCurrentIndex(idx);
    }

    // data flags
    ui->fetchAirQualityCheckBox->setChecked(settings->fetchAirQuality());
    ui->fetchUvCheckBox->setChecked(settings->fetchUV());
    ui->fetchPrecipProbCheckBox->setChecked(settings->fetchPrecipitationProbability());

    // updates
    ui->hourlyDisplaySpinBox->setValue(settings->hourlyDisplayHours());
    ui->refreshIntervalSpinBox->setValue(settings->refreshInterval());

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
    auto settings = m_ctx->settings();

    // language block
    {
        const QString code = ui->languageComboBox->currentData().toString();
        const auto lang = LanguageUtils::fromCode(code);
        settings->setLanguage(lang);
    }

    // theme block
    {
        const auto theme = static_cast<Theme>(ui->themeComboBox->currentData().toInt());
        m_ctx->themeManager()->setTheme(theme);
    }

    // providers
    if (ui->providerComboBox->isEnabled()) {
        const QString name = ui->providerComboBox->currentText();
        const auto id = toWeatherProvider(name);
        settings->setWeatherProvider(id);
    }

    if (ui->geocoderComboBox->isEnabled()) {
        const QString name = ui->geocoderComboBox->currentText();
        const auto id = toGeocoderProvider(name);
        settings->setGeocoderProvider(id);
    }

     // units block
    {
        const auto s = static_cast<UnitSystem>(ui->unitsComboBox->currentData().toInt());
        settings->setUnitSystem(s);
    }

    // data flags
    settings->setFetchAirQuality(ui->fetchAirQualityCheckBox->isChecked());
    settings->setFetchUV(ui->fetchUvCheckBox->isChecked());
    settings->setFetchPrecipitationProbability(ui->fetchPrecipProbCheckBox->isChecked());

    // updates
    settings->setHourlyDisplayHours(ui->hourlyDisplaySpinBox->value());
    settings->setRefreshInterval(ui->refreshIntervalSpinBox->value());

    accept();
}

void SettingsDialog::onCancelButtonClicked() {
    reject();
}
