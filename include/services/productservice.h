#pragma once
#include "models/types.h"
class ProductService { public: static QVector<Product> search(const QString &term = {}); static bool save(Product product, QString *error = nullptr); static bool softDelete(qint64 id, QString *error = nullptr); static bool adjustStock(qint64 id, int delta, const QString &reason, qint64 userId, QString *error = nullptr); };
