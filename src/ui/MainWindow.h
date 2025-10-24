#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include "model/Forecast.h"
#include "ApplicationContext.h"
#include "model/DataStatus.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(ApplicationContext *ctx, QWidget *parent = nullptr);
    ~MainWindow() override;

    void restoreLastLocation();

public slots:
    void displayForecast(const Forecast &forecast, const QString &title);
    void retranslate();
    void updateWeather(const Forecast &forecast);

private slots:
    void onChangeLocationButtonClicked();
    void onRefreshButtonClicked();
    void onSettingsButtonClicked();
    void onSettingsChanged();

signals:
    void refreshRequested();
    void openSettingsRequested();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    ApplicationContext *m_ctx;

    QString m_locationLabelText;
    Forecast m_lastForecast;

    void rebuildRecents();
    void setStatus(DataStatus status, const QString &txt);
};

#endif //MAINWINDOW_H
