#include <QMessageBox>
#include <QPushButton>

#include "MainWindow.h"
#include "./ui_mainwindow.h"
#include "Logger.h"
#include "SettingsManager.h"

MainWindow::MainWindow(ApplicationContext *ctx, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_ctx(ctx) {
    ui->setupUi(this);
    setWindowTitle("AtmoSense");
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
    ui->plainTextEdit->setPlainText(text);
}

void MainWindow::retranslate() {
    ui->retranslateUi(this);
}

void MainWindow::updateWeather(const Forecast &forecast) {
    Logger::info("MainWindow: updating weather UI.");

    // simple weather update below
    // @todo
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
