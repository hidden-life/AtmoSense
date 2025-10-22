#include "WeatherSummaryWidget.h"
#include "./ui_weathersummarywidget.h"
#include "ApplicationContext.h"
#include "Logger.h"
#include "SettingsManager.h"
#include "ThemeManager.h"
#include "UnitFormatter.h"
#include "model/UnitSystem.h"

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
        return;
    }

    const bool isDark = m_ctx->themeManager()->isDarkTheme();
    const QIcon icon = m_ctx->iconMapper()->map(weather.weatherCode, isDark);
    const UnitSystem unitSystem = m_ctx->settings()->unitSystem();

    ui->iconLabel->setPixmap(icon.pixmap(96, 96));
    ui->temperatureLabel->setText(UnitFormatter::temperature(weather.temperature, unitSystem));

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

    // beauty animation
    auto *effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);
    auto *animation = new QPropertyAnimation(effect, "opacity", this);
    animation->setDuration(250);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

WeatherSummaryWidget::~WeatherSummaryWidget() {
    delete ui;
}
