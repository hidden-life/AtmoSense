#include "SettingsDialog.h"
#include "SettingsManager.h"
#include "./ui_settingsdialog.h"

SettingsDialog::SettingsDialog(SettingsManager *settings, QWidget *parent) :
    QDialog(parent), ui(new Ui::SettingsDialog), m_settings(settings) {
    ui->setupUi(this);

    ui->tempUnitComboBox->setCurrentText(m_settings->temperatureUnit());
    ui->windSpeedUnitComboBox->setCurrentText(m_settings->windSpeedUnit());
    ui->themeComboBox->setCurrentText(m_settings->theme());
    ui->providerComboBox->setCurrentText(m_settings->provider());
    ui->refreshIntervalSpinBox->setValue(m_settings->refreshInterval());

    connect(ui->saveButton, &QPushButton::clicked, this, &SettingsDialog::onSaveButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &SettingsDialog::onCancelButtonClicked);
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::onSaveButtonClicked() {
    m_settings->setTemperatureUnit(ui->tempUnitComboBox->currentText());
    m_settings->setWindSpeedUnit(ui->windSpeedUnitComboBox->currentText());
    m_settings->setTheme(ui->themeComboBox->currentText());
    m_settings->setProvider(ui->providerComboBox->currentText());
    m_settings->setRefreshInterval(ui->refreshIntervalSpinBox->value());

    accept();
}

void SettingsDialog::onCancelButtonClicked() {
    reject();
}
