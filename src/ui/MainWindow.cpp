#include "MainWindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
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
