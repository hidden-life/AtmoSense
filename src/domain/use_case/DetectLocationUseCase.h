#ifndef DOMAIN_USE_CASE_DETECT_LOCATION_USE_CASE_H
#define DOMAIN_USE_CASE_DETECT_LOCATION_USE_CASE_H

#include <QMessageBox>

#include "Logger.h"
#include "PlatformUtils.h"
#include "SettingsManager.h"

class DetectLocationUseCase {
public:
    DetectLocationUseCase(SettingsManager *settings) : m_settings(settings) {}

    void operator()() {
        Logger::info("Detecting user location via IP...");
        const auto loc = PlatformUtils::detectLocation();
        if (!loc.has_value()) {
            QMessageBox::warning(nullptr, QObject::tr("Location detection."), QObject::tr("Unable to detect your location."));
            return;
        }

        const auto &info = *loc;

        const QString msg = QObject::tr("Detected location:\n%1\nLatitude: %2\nLongitude: %3\n\nUse this location?").
            arg(info.displayName()).
            arg(info.latitude, 0, 'f', 4).
            arg(info.longitude, 0, 'f', 4);

        const auto q = QMessageBox::question(
            nullptr,
            QObject::tr("Confirm location"),
            msg,
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes
            );

        if (q == QMessageBox::Yes) {
            m_settings->setLatitude(info.latitude);
            m_settings->setLongitude(info.longitude);

            QMessageBox::information(
                nullptr,
                QObject::tr("Location saved"),
                QObject::tr("Your location has been updated successfully.")
                );

            Logger::info(QString("Location updated: %1, %2").arg(info.latitude).arg(info.longitude));
        } else {
            Logger::info("User canceled detected location.");
            QMessageBox::information(nullptr, QObject::tr("Cancelled"), QObject::tr("Location updated cancelled."));
        }
    }

private:
    SettingsManager *m_settings;
};

#endif //DOMAIN_USE_CASE_DETECT_LOCATION_USE_CASE_H
