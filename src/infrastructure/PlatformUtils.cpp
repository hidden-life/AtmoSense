#include "PlatformUtils.h"

QString PlatformUtils::osName() {
#ifdef Q_OS_WIN
    return "Windows";
#elif defined(Q_OS_MACOS)
    return "MacOS";
#elif defined(Q_OS_LINUX)
    return "Linux";
#else
    return "Unknown OS";
#endif
}

void PlatformUtils::setupAutostart(bool enabled) {}