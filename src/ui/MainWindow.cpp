#include <QMessageBox>
#include <QPushButton>
#include <QMenu>
#include <QCloseEvent>
#include <QToolButton>

#include "MainWindow.h"

#include "./ui_mainwindow.h"
#include "DailyForecastWidget.h"
#include "LocationSearchDialog.h"
#include "Logger.h"
#include "SettingsManager.h"
#include "repository/IWeatherRepository.h"

MainWindow::MainWindow(ApplicationContext *ctx, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_ctx(ctx) {
    ui->setupUi(this);
    setWindowTitle("AtmoSense");
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    resize(720, 800);

    ui->weatherSummaryWidget->setContext(ctx);
    ui->hourlyForecastWidget->setContext(ctx);
    ui->dailyForecastWidget->setContext(ctx);

    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshButtonClicked);
    connect(ui->changeLocationButton, &QPushButton::clicked, this, &MainWindow::onChangeLocationButtonClicked);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);

    restoreLastLocation();

    connect(m_ctx->settings().get(), &SettingsManager::settingsChanged, this, &MainWindow::onSettingsChanged);

    rebuildRecents();
    statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::displayForecast(const Forecast &forecast, const QString &title) {
    QString text;
    text += "Location: " + title + "\n";
    text += QString("Now: %1 °C, wind %2 m/s, code %3\n")
        .arg(forecast.weather.temperature, 0, 'f', 1)
        .arg(forecast.weather.windSpeed, 0, 'f', 1)
        .arg(forecast.weather.weatherCode);

    if (!forecast.daily.empty()) {
        text += "Daily (first 3):\n";
        for (int i = 0; i < (int)std::min<size_t>(3, forecast.daily.size()); i++) {
            const auto &d = forecast.daily[i];
            text += QString("    %1: %2 °C, code %3").arg(d.timestamp.date().toString(Qt::ISODate))
                .arg(d.temperature, 0, 'f', 1).arg(d.weatherCode);
        }
    }
}

void MainWindow::retranslate() {
    ui->retranslateUi(this);

    if (!m_locationLabelText.isEmpty()) {
        ui->locationLabel->setText(m_locationLabelText);
    }

    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::updateWeather(const Forecast &forecast) {
    Logger::info("MainWindow: updating weather UI.");
    setStatus(DataStatus::Loading, tr("Updating forecast..."));
    m_lastForecast = forecast;

    if (!m_ctx) {
        Logger::warn("MainWindow: context is null, skipping update process.");
        return;
    }

    try {
        // update summary widget info
        ui->weatherSummaryWidget->update(forecast.weather);

        // update hourly weather data
        if (!forecast.hourly.empty()) {
            ui->hourlyForecastWidget->update(forecast.hourly);
        }

        // update daily weather data if exists
        if (!forecast.daily.empty()) {
            ui->dailyForecastWidget->update(forecast.daily);
        }

        if (!m_ctx->weatherRepository()->lastUsedCache()) {
            // from cache
            const QString time = m_ctx->weatherRepository()->lastUpdated().isValid() ?
                m_ctx->weatherRepository()->lastUpdated().toLocalTime().toString("HH:mm") :
                tr("Unknown");
            setStatus(DataStatus::Cached, tr("Loaded from cache at: %1").arg(time));
        } else {
            setStatus(DataStatus::Online, tr("Updated at: %1").arg(QTime::currentTime().toString("HH:mm")));
        }
    } catch (const std::exception &e) {
        QString msg = QString::fromStdString(e.what());
        if (msg.contains("internet", Qt::CaseInsensitive)) {
            setStatus(DataStatus::Offline, tr("No internet connection."));
        } else {
            setStatus(DataStatus::Error, msg);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (const auto settings = m_ctx->settings(); settings->showClosePrompt()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Exit application"));
        msgBox.setText(tr("Do you want to exit or minimize the app to tray?"));
        msgBox.setIcon(QMessageBox::Question);
        const QPushButton *minimizeButton = msgBox.addButton(tr("Minimize to tray"), QMessageBox::AcceptRole);
        const QPushButton *quitButton = msgBox.addButton(tr("Quit"), QMessageBox::DestructiveRole);
        const QPushButton *cancelButton = msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
        msgBox.exec();

        if (msgBox.clickedButton() == minimizeButton) {
            this->hide();
            event->ignore();
            return;
        }

        if (msgBox.clickedButton() == quitButton) {
            QApplication::quit();
            return;
        }

        if (msgBox.clickedButton() == cancelButton) {
            event->ignore();
        }

        event->ignore();
        return;
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::rebuildRecents() {
    auto *menu = new QMenu(this);
    const auto recents = m_ctx->settings()->recentLocations();
    if (recents.empty()) {
        auto *action = new QAction(tr("No recent locations."), menu);
        action->setEnabled(false);
        menu->addAction(action);
    } else {
        for (const auto &recent : recents) {
            auto *act = new QAction(QString("%1 - %2%3").arg(recent.name).arg(recent.country).arg(recent.region.isEmpty() ? "" : (", " + recent.region)), menu);
            connect(act, &QAction::triggered, this, [this, recent] {
                m_ctx->settings()->setLastLocation(recent);
                m_ctx->settings()->addRecentLocation(recent);
                ui->locationLabel->setText(recent.name);

                emit refreshRequested();
            });

            menu->addAction(act);
        }

        ui->recentButton->setMenu(menu);
        ui->recentButton->setPopupMode(QToolButton::InstantPopup);
    }
}

void MainWindow::setStatus(DataStatus status, const QString &txt) {
    const QString color = toColor(status);
    const QString text = toEmoji(status) + (txt.isEmpty() ? "" : " • " + txt);

    ui->statusLabel->setText(text);
    ui->statusLabel->setStyleSheet(QString("color: %1; font-weight: 500; font-size: 13px;").arg(color));

    // animation
    auto *effect = new QGraphicsOpacityEffect(this);
    ui->statusLabel->setGraphicsEffect(effect);
    auto *animation = new QPropertyAnimation(effect, "opacity", this);
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::restoreLastLocation() {
    const auto location = m_ctx->settings()->lastLocation();
    if (!location.name.isEmpty()) {
        m_locationLabelText = location.name;
        ui->locationLabel->setText(location.name);
    } else {
        m_locationLabelText = tr("Select location");
        ui->locationLabel->setText(m_locationLabelText);
    }
}

void MainWindow::onChangeLocationButtonClicked() {
    if (LocationSearchDialog dialog(m_ctx, this); dialog.exec() == QDialog::Accepted) {
        const auto location = dialog.selected();
        m_ctx->settings()->setLastLocation(location);
        m_locationLabelText = location.name;
        ui->locationLabel->setText(location.name);

        m_ctx->settings()->addRecentLocation(location);
        rebuildRecents();

        emit refreshRequested();
    }
}

void MainWindow::onRefreshButtonClicked() {
    emit refreshRequested();
}

void MainWindow::onSettingsButtonClicked() {
    emit openSettingsRequested();
}

void MainWindow::onSettingsChanged() {
    if (!m_lastForecast.hourly.empty() || !m_lastForecast.daily.empty() || m_lastForecast.weather.timestamp.isValid()) {
        updateWeather(m_lastForecast);
    }
}
