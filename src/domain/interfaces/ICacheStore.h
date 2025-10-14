#ifndef DOMAIN_INTERFACES_ICACHE_STORE_H
#define DOMAIN_INTERFACES_ICACHE_STORE_H

#include <optional>
#include <QString>
#include <QByteArray>

class ICacheStore {
public:
    virtual ~ICacheStore() = default;
    virtual void put(const QString &key, const QByteArray &value, int ttl) = 0;
    virtual std::optional<QByteArray> get(const QString &key) = 0;
    virtual void clear() = 0;
};

#endif //DOMAIN_INTERFACES_ICACHE_STORE_H
