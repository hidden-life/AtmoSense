#ifndef DOMAIN_MODEL_UNIT_SYSTEM_H
#define DOMAIN_MODEL_UNIT_SYSTEM_H

enum class UnitSystem {
    Metric,
    Imperial,
};

inline QString toString(const UnitSystem unit) {
    return unit == UnitSystem::Metric ? "metric" : "imperial";
}

inline UnitSystem toUnitSystem(const QString &unit) {
    if (unit == "imperial" || unit == "Imperial") {
        return UnitSystem::Imperial;
    }

    return UnitSystem::Metric;
}

#endif //DOMAIN_MODEL_UNIT_SYSTEM_H
