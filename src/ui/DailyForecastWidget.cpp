#include "DailyForecastWidget.h"
#include "./ui_dailyforecastwidget.h"
#include "ApplicationContext.h"
#include "ThemeManager.h"
#include "../application/IconMapper.h"

DailyForecastWidget::DailyForecastWidget(QWidget *parent) : QWidget(parent), ui(new Ui::DailyForecastWidget) {
    ui->setupUi(this);
}

DailyForecastWidget::DailyForecastWidget(ApplicationContext *ctx, QWidget *parent) :
    QWidget(parent), ui(new Ui::DailyForecastWidget), m_ctx(ctx) {
    ui->setupUi(this);
}

void DailyForecastWidget::setContext(ApplicationContext *ctx) {
    m_ctx = ctx;
}

void DailyForecastWidget::update(const std::vector<Weather> &daily) {
    auto *list = ui->listLayout;

    // clear before
    while (QLayoutItem *item = list->takeAt(0)) {
        if (auto *w = item->widget()) {
            w->deleteLater();
        }
        delete item;
    }

    QApplication::processEvents();

    const bool isDark = m_ctx->themeManager()->isDarkTheme();

    for (const auto &d : daily) {
        auto *row = new QWidget();
        auto *h = new QHBoxLayout(row);
        h->setContentsMargins(8, 4, 8, 4);

        const QIcon icon = IconMapper::map(d.weatherCode, isDark);
        auto *iconLabel = new QLabel();
        iconLabel->setPixmap(icon.pixmap(28, 28));

        auto *dateLabel = new QLabel(d.timestamp.toLocalTime().toString("ddd, dd MMM"));
        auto *temperatureLabel = new QLabel(QString("%1 °C").arg(d.temperature, 0, 'f', 1));

        h->addWidget(iconLabel);
        h->addWidget(dateLabel);
        h->addStretch();
        h->addWidget(temperatureLabel);

        row->setLayout(h);
        list->addWidget(row);
    }
}


DailyForecastWidget::~DailyForecastWidget() {
    delete ui;
}
