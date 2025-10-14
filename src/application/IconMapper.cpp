#include "IconMapper.h"

QIcon IconMapper::iconFor(int weatherCode, bool isDarkTheme) {
    if (weatherCode == 0) {
        return QIcon(":/icons/sunny.svg");
    } else {
        return QIcon(":/icons/cloudy.svg");
    }
}