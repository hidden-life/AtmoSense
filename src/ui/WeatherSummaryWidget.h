#ifndef WEATHERSUMMARYWIDGET_H
#define WEATHERSUMMARYWIDGET_H

#include <QWidget>

#include "model/Weather.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WeatherSummaryWidget; }
QT_END_NAMESPACE

class ApplicationContext;

class WeatherSummaryWidget final : public QWidget {
Q_OBJECT

public:
    explicit WeatherSummaryWidget(QWidget *parent = nullptr);
    explicit WeatherSummaryWidget(ApplicationContext *ctx, QWidget *parent = nullptr);
    ~WeatherSummaryWidget() override;

    void setContext(ApplicationContext *ctx);
    void update(const Weather &weather);

private:
    Ui::WeatherSummaryWidget *ui;
    ApplicationContext *m_ctx;
};


#endif //WEATHERSUMMARYWIDGET_H
