#ifndef DOMAIN_MODEL_DATA_STATUS_H
#define DOMAIN_MODEL_DATA_STATUS_H

#include <QString>

enum class DataStatus {
    Online,
    Offline,
    Cached,
    Error,
    Loading,
};

inline QString toEmoji(DataStatus s) {
    switch (s) {
        case DataStatus::Online: return "🌐 Online";
        case DataStatus::Cached: return "📦 Cached";
        case DataStatus::Offline: return "🔌 Offline";
        case DataStatus::Error: return "⚠️ Error";
        case DataStatus::Loading: return "⏳ Updating...";
    }

    return "";
}

inline QString toColor(DataStatus s) {
    switch (s) {
        case DataStatus::Online: return "#2ecc71";
        case DataStatus::Cached: return "#f1c40f";
        case DataStatus::Offline: return "#95a5a6";
        case DataStatus::Error: return "#e74c3c";
        case DataStatus::Loading: return "#3498db";
    }

    return "#666";
}

#endif //DOMAIN_MODEL_DATA_STATUS_H
