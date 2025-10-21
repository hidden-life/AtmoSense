#ifndef HOURLYFORECASTWIDGET_H
#define HOURLYFORECASTWIDGET_H

#include <QWidget>
#include <QLabel>

#include "ApplicationContext.h"
#include "model/Weather.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HourlyForecastWidget; }
QT_END_NAMESPACE

class HourlyForecastWidget : public QWidget {
Q_OBJECT

public:
    explicit HourlyForecastWidget(QWidget *parent = nullptr);
    explicit HourlyForecastWidget(ApplicationContext *ctx, QWidget *parent = nullptr);
    ~HourlyForecastWidget() override;

    void update(const std::vector<Weather> &hourly);
    void setContext(ApplicationContext *ctx);

private:
    Ui::HourlyForecastWidget *ui;
    ApplicationContext *m_ctx = nullptr;
    std::vector<Weather> m_lastHourly;
};

#endif //HOURLYFORECASTWIDGET_H
