#include "IconMapper.h"

QIcon IconMapper::iconFor(int weatherCode, bool isDarkTheme) {
    if (weatherCode == 0) {
        return QIcon(":/icons/sunny.png");
    } else {
        return QIcon(":/icons/cloudy.png");
    }
}