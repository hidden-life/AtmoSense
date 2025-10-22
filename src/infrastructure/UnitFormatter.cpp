#include "UnitFormatter.h"

static inline double c2f(double v) { return v * 9.0 / 5.0 + 32.0; }
static inline double ms2mph(double v) { return v * 2.23693629; }
static inline double hpa2inhg(double v) { return v * 0.0295299830714; }

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
