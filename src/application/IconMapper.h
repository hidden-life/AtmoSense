#ifndef APPLICATION_ICON_MAPPER_H
#define APPLICATION_ICON_MAPPER_H

#include <QIcon>

class IconMapper {
public:
    static QIcon iconFor(int weatherCode, bool isDarkTheme = false);
    static QIcon map(int weatherCode, bool isDarkTheme = false);

private:
    static QString resolveIconPath(int weatherCode, bool isDarkTheme);
    static bool isNightTime();
};

#endif //APPLICATION_ICON_MAPPER_H
