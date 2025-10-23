#include "UnitFormatter.h"

static inline double c2f(const double v) { return v * 9.0 / 5.0 + 32.0; }
static inline double ms2mph(const double v) { return v * 2.23693629; }
static inline double hpa2inhg(const double v) { return v * 0.0295299830714; }
static inline double mm2in(const double mm) { return mm * 0.3937; }

QString UnitFormatter::temperature(const double celsius, const UnitSystem s, const int precision) {
    if (s == UnitSystem::Imperial) {
        return QString::number(c2f(celsius), 'f', precision) + "°F";
    }

    return QString::number(celsius, 'f', precision) + "°C";
}

QString UnitFormatter::windSpeed(const double ms, const UnitSystem s, const int precision) {
    if (s == UnitSystem::Imperial) {
        return QString::number(ms2mph(ms), 'f', precision) + " mph";
    }

    return QString::number(ms, 'f', precision) + " m/s";
}

QString UnitFormatter::pressure(const double h, const UnitSystem s, const int precision) {
    if (s == UnitSystem::Imperial) {
        return QString::number(hpa2inhg(h), 'f', precision) + " inHg";
    }

    return QString::number(h, 'f', precision) + " hPa";
}

QString UnitFormatter::precipitation(const double mm, const UnitSystem s, const int precision) {
    if (s == UnitSystem::Imperial) {
        return QString::number(mm2in(mm), 'f', precision) + " in";
    }

    return QString::number(mm, 'f', precision) + " mm";
}

QString UnitFormatter::uv(const double index, const int precision) {
    return QString::number(index, 'f', precision);
}

QString UnitFormatter::probability(double p, int precision) {
    return QString::number(p, 'f', precision) + "%";
}
