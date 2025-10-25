#include "OpenWeatherMapWeatherProvider.h"

#include "Logger.h"

OpenWeatherMapWeatherProvider::OpenWeatherMapWeatherProvider(NetworkClient &client, QString apiKey) : m_client(client), m_apiKey(std::move(apiKey)) {}

Forecast OpenWeatherMapWeatherProvider::fetch(double lat, double lon, const QString &tz) {
    if (m_apiKey.isEmpty()) {
        throw std::runtime_error("OpenWeatherMap: API key missing.");
    }

    Forecast forecast;
    const QString url = QString("https://api.openweathermap.org/data/2.5/onecall?lat=%1&lon=%2&appid=%3&units=metric").arg(lat).arg(lon).arg(m_apiKey);
    const auto data = m_client.getJson(url);
    if (data.isEmpty()) {
        Logger::warning("OpenWeatherMap: Failed to fetch weather data.");
        throw std::runtime_error("Failed to fetch weather data.");
    }

    const auto doc = QJsonDocument::fromJson(data.toJson());
    const auto root = doc.object();

    // current
    const auto current = root.value("current").toObject();
    Weather w;
    w.temperature = current.value("temp").toDouble();
    w.humidity = current.value("humidity").toDouble();
    w.pressure = current.value("pressure").toDouble();
    w.windSpeed = current.value("wind_speed").toDouble();
    w.timestamp = QDateTime::fromSecsSinceEpoch(current.value("dt").toInt());
    forecast.weather = w;

    // hourly
    const auto hourlyArr = root.value("hourly").toArray();
    for (const auto &h : hourlyArr) {
        const auto o = h.toObject();
        Weather hourlyWeather;
        hourlyWeather.temperature = o.value("temp").toDouble();
        hourlyWeather.humidity = o.value("humidity").toDouble();
        hourlyWeather.pressure = o.value("pressure").toDouble();
        hourlyWeather.windSpeed = o.value("wind_speed").toDouble();
        hourlyWeather.timestamp = QDateTime::fromSecsSinceEpoch(o.value("dt").toInt());

        forecast.hourly.push_back(hourlyWeather);
    }

    // daily
    const auto dailyArr = root.value("daily").toArray();
    for (const auto &d : dailyArr) {
        const auto o = d.toObject();
        Weather dailyWeather;
        dailyWeather.temperature = o.value("temp").toObject().value("day").toDouble();
        dailyWeather.humidity = o.value("humidity").toDouble();
        dailyWeather.pressure = o.value("pressure").toDouble();
        dailyWeather.windSpeed = o.value("wind_speed").toDouble();
        dailyWeather.timestamp = QDateTime::fromSecsSinceEpoch(o.value("dt").toInt());

        forecast.daily.push_back(dailyWeather);
    }

    Logger::info("Fetched weather from OpenWeatherMap successfully.");

    return forecast;
}

bool OpenWeatherMapWeatherProvider::apiKeyRequired() {
    return true;
}

