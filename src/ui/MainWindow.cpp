#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>

#include "MainWindow.h"
#include "./ui_mainwindow.h"
#include "LocationSearchDialog.h"
#include "Logger.h"
#include "SettingsManager.h"

MainWindow::MainWindow(ApplicationContext *ctx, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_ctx(ctx) {
    ui->setupUi(this);
    setWindowTitle("AtmoSense");
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    resize(720, 800);

    ui->weatherSummaryWidget->setContext(ctx);
    ui->hourlyForecastWidget->setContext(ctx);
    ui->dailyForecastWidget->setContext(ctx);

    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshButtonClicked);
    connect(ui->changeLocationButton, &QPushButton::clicked, this, &MainWindow::onChangeLocationButtonClicked);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);

    restoreLastLocation();

    statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::displayForecast(const Forecast &forecast, const QString &title) {
    QString text;
    text += "Location: " + title + "\n";
    text += QString("Now: %1 °C, wind %2 m/s, code %3\n")
        .arg(forecast.weather.temperature, 0, 'f', 1)
        .arg(forecast.weather.windSpeed, 0, 'f', 1)
        .arg(forecast.weather.weatherCode);

    if (!forecast.daily.empty()) {
        text += "Daily (first 3):\n";
        for (int i = 0; i < (int)std::min<size_t>(3, forecast.daily.size()); i++) {
            const auto &d = forecast.daily[i];
            text += QString("    %1: %2 °C, code %3").arg(d.timestamp.date().toString(Qt::ISODate))
                .arg(d.temperature, 0, 'f', 1).arg(d.weatherCode);
        }
    }
}

void MainWindow::retranslate() {
    ui->retranslateUi(this);

    if (!m_locationLabelText.isEmpty()) {
        ui->locationLabel->setText(m_locationLabelText);
    }

    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::updateWeather(const Forecast &forecast) {
    Logger::info("MainWindow: updating weather UI.");

    if (!m_ctx) {
        Logger::warn("MainWindow: context is null, skipping update process.");
        return;
    }

    ui->weatherSummaryWidget->update(forecast.weather);

    // update hourly weather data
    if (!forecast.hourly.empty()) {
        ui->hourlyForecastWidget->update(forecast.hourly);
    }

    // update daily weather data if exists
    if (!forecast.daily.empty()) {
        ui->dailyForecastWidget->update(forecast.daily);
    }

    statusBar()->showMessage(tr("Updated: %1").arg(QTime::currentTime().toString("HH:mm")));
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (const auto settings = m_ctx->settings(); settings->showClosePrompt()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Exit application"));
        msgBox.setText(tr("Do you want to exit or minimize the app to tray?"));
        msgBox.setIcon(QMessageBox::Question);
        const QPushButton *minimizeButton = msgBox.addButton(tr("Minimize to tray"), QMessageBox::AcceptRole);
        const QPushButton *quitButton = msgBox.addButton(tr("Quit"), QMessageBox::DestructiveRole);
        const QPushButton *cancelButton = msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
        msgBox.exec();

        if (msgBox.clickedButton() == minimizeButton) {
            this->hide();
            event->ignore();
            return;
        }

        if (msgBox.clickedButton() == quitButton) {
            QApplication::quit();
            return;
        }

        if (msgBox.clickedButton() == cancelButton) {
            event->ignore();
        }

        event->ignore();
        return;
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::restoreLastLocation() {
    const auto location = m_ctx->settings()->lastLocation();
    if (!location.name.isEmpty()) {
        m_locationLabelText = location.name;
        ui->locationLabel->setText(location.name);
    } else {
        m_locationLabelText = tr("Select location");
        ui->locationLabel->setText(m_locationLabelText);
    }
}

void MainWindow::onChangeLocationButtonClicked() {
    if (LocationSearchDialog dialog(m_ctx, this); dialog.exec() == QDialog::Accepted) {
        const auto location = dialog.selected();
        m_ctx->settings()->setLastLocation(location);
        m_locationLabelText = location.name;
        ui->locationLabel->setText(location.name);

        emit refreshRequested();
    }
}

void MainWindow::onRefreshButtonClicked() {
    emit refreshRequested();
}

void MainWindow::onSettingsButtonClicked() {
    emit openSettingsRequested();
}
