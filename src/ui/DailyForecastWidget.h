#ifndef DAILYFORECASTWIDGET_H
#define DAILYFORECASTWIDGET_H

#include <QWidget>

#include "model/Weather.h"

class ApplicationContext;

QT_BEGIN_NAMESPACE
namespace Ui { class DailyForecastWidget; }
QT_END_NAMESPACE

class DailyForecastWidget final : public QWidget {
Q_OBJECT

public:
    explicit DailyForecastWidget(QWidget *parent = nullptr);
    explicit DailyForecastWidget(ApplicationContext *ctx, QWidget *parent = nullptr);
    ~DailyForecastWidget() override;

    void update(const std::vector<Weather> &daily);
    void setContext(ApplicationContext *ctx);

private:
    Ui::DailyForecastWidget *ui;
    ApplicationContext *m_ctx;
};


#endif //DAILYFORECASTWIDGET_H
