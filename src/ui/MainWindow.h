#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "model/Forecast.h"
#include "ApplicationContext.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(ApplicationContext *ctx, QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void displayForecast(const Forecast &forecast, const QString &title);
    void retranslate();
    void updateWeather(const Forecast &forecast);

private slots:
    void onChangeLocationButtonClicked();
    void onRefreshButtonClicked();

signals:
    void refreshRequested();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    ApplicationContext *m_ctx;

    void restoreLastLocation();
};

#endif //MAINWINDOW_H
