#include <QFile>
#include <QDateTime>
#include <QStandardPaths>
#include <QCryptographicHash>

#include "FileCacheStore.h"

#include "Logger.h"

static QString hash(const QString &key) {
    return QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha256).toHex();
}

FileCacheStore::FileCacheStore(const QDir &baseDir) : m_dir(baseDir) {
    if (!m_dir.exists()) {
        m_dir.mkpath(".");
    }
}

QString FileCacheStore::pathFor(const QString &key) const {
    return m_dir.filePath(hash(key) + ".cache");
}

void FileCacheStore::put(const QString &key, const QByteArray &value, int ttl) {
    QMutexLocker mtxLocker(&m_mutex);

    if (QFile file(pathFor(key)); file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        const qint64 expiry = QDateTime::currentSecsSinceEpoch() + ttl;
        file.write(reinterpret_cast<const char*>(&expiry), sizeof(expiry));
        file.write(value);
        file.close();
        Logger::info("Write cache for key: <" + key + "> with value: " + value);
    };
}

std::optional<QByteArray> FileCacheStore::get(const QString &key) {
    QMutexLocker mtxLocker(&m_mutex);
    QFile file(pathFor(key));
    Logger::debug("Cache path: " + pathFor(key));
    if (!file.open(QIODevice::ReadOnly)) {
        return std::nullopt;
    }

    qint64 expiry = 0;
    if (file.read(reinterpret_cast<char*>(&expiry), sizeof(expiry)) != sizeof(expiry)) {
        file.close();
        file.remove();
        return std::nullopt;
    }

    if (QDateTime::currentSecsSinceEpoch() > expiry) {
        file.close();
        file.remove();
        return std::nullopt;
    }

    Logger::info("Receive cache for key: " + key);

    QByteArray payload = file.readAll();
    file.close();

    return payload;
}

void FileCacheStore::clear() {
    for (const QFileInfo &info : m_dir.entryInfoList(QStringList() << "*.cache", QDir::Files)) {
        QFile::remove(info.absoluteFilePath());
    }
}

bool FileCacheStore::isExpired(const QString &path) const {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return true;

    qint64 expiry = 0;
    if (file.read(reinterpret_cast<char*>(&expiry), sizeof(expiry)) != sizeof(expiry)) {
        file.close();
        return true;
    }
    file.close();

    return QDateTime::currentSecsSinceEpoch() > expiry;
}
