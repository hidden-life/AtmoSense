#ifndef DATA_CACHE_FILE_CACHE_STORE_H
#define DATA_CACHE_FILE_CACHE_STORE_H

#include <QDir>
#include "interfaces/ICacheStore.h"

class FileCacheStore : public ICacheStore {
    QDir m_dir;
public:
    explicit FileCacheStore(const QDir &baseDir);
    void put(const QString &key, const QByteArray &value, int ttl) override;
    std::optional<QByteArray> get(const QString &key) override;
    void clear() override;

private:
    QString pathFor(const QString &key) const;
};

#endif //DATA_CACHE_FILE_CACHE_STORE_H
