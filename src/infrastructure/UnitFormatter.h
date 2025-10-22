#ifndef INFRASTRUCTURE_UNIT_FORMATTER_H
#define INFRASTRUCTURE_UNIT_FORMATTER_H

#include <QString>

#include "model/UnitSystem.h"

class UnitFormatter {
public:
    static QString temperature(double celsius, UnitSystem s, int precision = 1);
    static QString windSpeed(double ms, UnitSystem s, int precision = 1);
    static QString pressure(double h, UnitSystem s, int precision = 0);
};

#endif //INFRASTRUCTURE_UNIT_FORMATTER_H
