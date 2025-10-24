#include <QPropertyAnimation>
#include <QComboBox>

#include "SettingsDialog.h"
#include "SettingsManager.h"
#include "./ui_settingsdialog.h"
#include "ApplicationContext.h"
#include "ThemeManager.h"
#include "interfaces/IGeocoder.h"
#include "interfaces/IWeatherProvider.h"
#include "model/Provider.h"

SettingsDialog::SettingsDialog(ApplicationContext *ctx, QWidget *parent) :
    QDialog(parent), ui(new Ui::SettingsDialog), m_ctx(ctx) {
    ui->setupUi(this);

    populateProviders();

    ui->apiKeyLabel->setVisible(false);
    ui->apiKeyLineEdit->setVisible(false);

    // animation
    auto toggleVisibility = [&](QWidget *widget, bool isVisible) {
        auto *animation = new QPropertyAnimation(widget, "maximumHeight", widget);
        animation->setDuration(200);
        if (isVisible) {
            widget->setVisible(true);
            animation->setStartValue(0);
            animation->setEndValue(widget->sizeHint().height());
        } else {
            animation->setStartValue(widget->height());
            animation->setEndValue(0);
            connect(animation, &QPropertyAnimation::finished, widget, [widget]() {
                widget->setVisible(false);
            });
        }

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    };

    connect(ui->providerComboBox, &QComboBox::currentTextChanged, this, [this, toggleVisibility](const QString &provider) {
        const auto &providers = m_ctx->weatherProviders();
        const auto it = std::find_if(providers.begin(), providers.end(), [&](const auto &p) {
            return p.second->name() == provider;
        });

        if (it != providers.end()) {
            const bool keyRequired = it->second->apiKeyRequired();

            toggleVisibility(ui->apiKeyLabel, keyRequired);
            toggleVisibility(ui->apiKeyLineEdit, keyRequired);

            if (keyRequired) {
                ui->apiKeyLineEdit->setText(m_ctx->settings()->openWeatherMapAPIKey());
            }
        }
    });

    QMetaObject::invokeMethod(ui->providerComboBox, [this] {
        emit ui->providerComboBox->currentIndexChanged(ui->providerComboBox->currentIndex());
    }, Qt::QueuedConnection);

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
    ui->apiKeyLabel->setVisible(false);
    ui->apiKeyLineEdit->setVisible(false);

    ui->providerComboBox->clear();
    ui->geocoderComboBox->clear();

    // add providers
    for (const auto &pair : m_ctx->weatherProviders()) {
        auto provider = pair.second;
        if (!provider) return;

        ui->providerComboBox->addItem(provider->name(), provider->name());
    }

    for (const auto &pair : m_ctx->geocoderProviders()) {
        auto provider = pair.second;
        if (!provider) return;

        ui->geocoderComboBox->addItem(provider->name(), provider->name());
    }

    // determine active providers from settings
    const WeatherProviderId activeWeather = m_ctx->settings()->weatherProvider();
    const GeocoderProviderId activeGeocoder = m_ctx->settings()->geocoderProvider();

    const QString activeWeatherName = toString(activeWeather);
    const QString activeGeocoderName = toString(activeGeocoder);

    int weatherIdx = ui->providerComboBox->findData(activeWeatherName);
    if (weatherIdx >= 0) {
        ui->providerComboBox->setCurrentIndex(weatherIdx);
    } else if (ui->providerComboBox->currentIndex() > 0) {
        ui->providerComboBox->setCurrentIndex(0);
    }

    int geocoderIdx = ui->geocoderComboBox->findData(activeGeocoderName);
    if (geocoderIdx >= 0) {
        ui->geocoderComboBox->setCurrentIndex(geocoderIdx);
    } else if (ui->geocoderComboBox->currentIndex() > 0) {
        ui->geocoderComboBox->setCurrentIndex(0);
    }

    // manage access for combo-boxes
    ui->providerComboBox->setEnabled(m_ctx->weatherProviderCount() > 0);
    ui->geocoderComboBox->setEnabled(m_ctx->geocoderProviderCount() > 0);

    updateAPIKeyVisibility();
}

void SettingsDialog::updateAPIKeyVisibility() {
    const QString name = ui->providerComboBox->currentData().toString();

    const auto &providers = m_ctx->weatherProviders();
    auto it = std::find_if(providers.begin(), providers.end(), [&](const auto &p){ return p.second->name() == name; });
    const bool keyRequired = (it != providers.end()) ? it->second->apiKeyRequired() : false;

    ui->apiKeyLabel->setVisible(keyRequired);
    ui->apiKeyLineEdit->setVisible(keyRequired);

    if (keyRequired) {
        ui->apiKeyLineEdit->setText(m_ctx->settings()->openWeatherMapAPIKey());
    } else {
        ui->apiKeyLineEdit->clear();
    }
}

void SettingsDialog::onSaveButtonClicked() {
    const auto settings = m_ctx->settings();

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

    // API key
    if (const QString selectedProvider = ui->providerComboBox->currentText(); selectedProvider.contains("OpenWeatherMap", Qt::CaseInsensitive)) {
        m_ctx->settings()->setOpenWeatherMapAPIKey(ui->apiKeyLineEdit->text());
    }

    accept();
}

void SettingsDialog::onCancelButtonClicked() {
    reject();
}
