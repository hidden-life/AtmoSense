#include "WeatherSummaryWidget.h"
#include "./ui_weathersummarywidget.h"
#include "ApplicationContext.h"
#include "Logger.h"
#include "ThemeManager.h"
#include "../application/IconMapper.h"

WeatherSummaryWidget::WeatherSummaryWidget(QWidget *parent) : QWidget(parent), ui(new Ui::WeatherSummaryWidget) {
    ui->setupUi(this);
}

WeatherSummaryWidget::WeatherSummaryWidget(ApplicationContext *ctx, QWidget *parent) :
    QWidget(parent), ui(new Ui::WeatherSummaryWidget), m_ctx(ctx) {
    ui->setupUi(this);
}

void WeatherSummaryWidget::setContext(ApplicationContext *ctx) {
    m_ctx = ctx;
}

void WeatherSummaryWidget::update(const Weather &weather) {
    if (!m_ctx) {
        Logger::warn("WeatherSummaryWidget::update: context is null.");
    }

    const bool isDark = m_ctx->themeManager()->isDarkTheme();
    const QIcon icon = IconMapper::map(weather.weatherCode, isDark);

    ui->iconLabel->setPixmap(icon.pixmap(96, 96));
    ui->temperatureLabel->setText(QString::number(weather.temperature, 'f', 1) + "°C"); // @todo Change to temperature unit from settings!

    QString description;
    switch (weather.weatherCode) {
        case 0: description = tr("Clear sky"); break;
        case 1: description = tr("Mainly clear"); break;
        case 2: description = tr("Partly clear"); break;
        case 3: description = tr("Overcast"); break;
        case 45:
        case 48: description = tr("Fog"); break;
        default: description = tr("Unknown"); break;
    }
    ui->descriptionLabel->setText(description);

    ui->extraLabel->setText(tr("Humidity: %1% | Wind: %2 m/s | Pressure: %3 hPa")
        .arg(weather.humidity, 0, 'f', 0)
        .arg(weather.windSpeed, 0, 'f', 1)
        .arg(weather.pressure, 0, 'f', 0));
}

WeatherSummaryWidget::~WeatherSummaryWidget() {
    delete ui;
}
