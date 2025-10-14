#include <QFile>
#include <QDateTime>
#include <QStandardPaths>
#include <QCryptographicHash>

#include "FileCacheStore.h"

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
    QFile file(pathFor(key));
    if (file.open(QIODevice::WriteOnly)) {
        qint64 expiry = QDateTime::currentSecsSinceEpoch() + ttl;
        file.write(reinterpret_cast<const char*>(&expiry), sizeof(expiry));
        file.write(value);
        file.close();
    }
}

std::optional<QByteArray> FileCacheStore::get(const QString &key) {
    QFile file(pathFor(key));
    if (!file.open(QIODevice::ReadOnly)) {
        return std::nullopt;
    }

    qint64 expiry = 0;
    if (file.read(reinterpret_cast<char*>(&expiry), sizeof(expiry)) != sizeof(expiry)) {
        file.close();
        return std::nullopt;
    }

    if (QDateTime::currentSecsSinceEpoch() > expiry) {
        file.close();
        file.remove();
        return std::nullopt;
    }

    QByteArray payload = file.readAll();
    file.close();

    return payload;
}

void FileCacheStore::clear() {
    for (const QFileInfo &info : m_dir.entryInfoList(QStringList() << "*.cache", QDir::Files)) {
        QFile::remove(info.absoluteFilePath());
    }
}
