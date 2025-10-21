#include "HourlyForecastWidget.h"
#include "./ui_hourlyforecastwidget.h"
#include "SettingsManager.h"
#include "ThemeManager.h"
#include "../application/IconMapper.h"

HourlyForecastWidget::HourlyForecastWidget(QWidget *parent) : QWidget(parent), ui(new Ui::HourlyForecastWidget) {
    ui->setupUi(this);
}

HourlyForecastWidget::HourlyForecastWidget(ApplicationContext *ctx, QWidget *parent) :
    QWidget(parent), ui(new Ui::HourlyForecastWidget), m_ctx(ctx) {
    ui->setupUi(this);

    connect(m_ctx->settings().get(), &SettingsManager::hourlyDisplayHoursChanged, this, [this]() {
        if (!m_lastHourly.empty()) {
            update(m_lastHourly);
        }
    });
}

void HourlyForecastWidget::update(const std::vector<Weather> &hourly) {
    if (!m_ctx) return;
    const auto *settings = m_ctx->settings().get();
    const int hours = settings->hourlyDisplayHours();

    QLayout *box = ui->containerWidget->layout();
    if (!box) return;

    QLayoutItem *item;
    while ((item = box->takeAt(0))) {
        if (auto *w = item->widget()) {
            w->deleteLater();
        }
        delete item;
    }

    QApplication::processEvents();

    m_lastHourly = hourly;

    const int count = std::min<int>(hourly.size(), hours);
    const bool isDark = m_ctx->themeManager()->isDarkTheme();
    for (int i = 0; i < count; i++) {
        const auto &h = hourly[i];
        auto *card = new QWidget();
        auto *v = new QVBoxLayout(card);
        v->setAlignment(Qt::AlignCenter);

        const QIcon icon = IconMapper::map(h.weatherCode, isDark);
        auto *iconLabel = new QLabel();
        iconLabel->setPixmap(icon.pixmap(48, 48));

        auto *timeLabel = new QLabel(h.timestamp.toLocalTime().toString("HH:mm"));
        auto *temperatureLabel = new QLabel(QString::number(h.temperature, 'f', 1) + "°C");
        timeLabel->setAlignment(Qt::AlignCenter);
        temperatureLabel->setAlignment(Qt::AlignCenter);
        temperatureLabel->setStyleSheet("font-weight: 500;");

        v->addWidget(iconLabel);
        v->addWidget(timeLabel);
        v->addWidget(temperatureLabel);

        card->setLayout(v);
        card->setStyleSheet("border-radius: 8px; background: rgba(180, 180, 180, 0.12); padding: 6px;");

        box->addWidget(card);
    }
}

void HourlyForecastWidget::setContext(ApplicationContext *ctx) {
    m_ctx = ctx;
}


HourlyForecastWidget::~HourlyForecastWidget() {
    delete ui;
}
