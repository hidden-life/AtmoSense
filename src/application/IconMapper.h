#ifndef APPLICATION_ICON_MAPPER_H
#define APPLICATION_ICON_MAPPER_H

#include <QIcon>

class IconMapper {
public:
    static QIcon iconFor(int weatherCode, bool isDarkTheme);
};

#endif //APPLICATION_ICON_MAPPER_H
