#include "HourlyForecastWidget.h"
#include "./ui_hourlyforecastwidget.h"
#include "ThemeManager.h"
#include "../application/IconMapper.h"

HourlyForecastWidget::HourlyForecastWidget(QWidget *parent) : QWidget(parent), ui(new Ui::HourlyForecastWidget) {
    ui->setupUi(this);
    // ui->scrollArea->setWidgetResizable(true);
    // ui->containerWidget->setFixedHeight(200);
    // ui->listLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

HourlyForecastWidget::HourlyForecastWidget(ApplicationContext *ctx, QWidget *parent) :
    QWidget(parent), ui(new Ui::HourlyForecastWidget), m_ctx(ctx) {
    ui->setupUi(this);
}

void HourlyForecastWidget::update(const std::vector<Weather> &hourly) {
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

    const bool isDark = m_ctx->themeManager()->isDarkTheme();
    for (const auto &h : hourly) {
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
