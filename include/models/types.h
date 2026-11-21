#pragma once
#include <QString>
#include <QVector>
struct Product { qint64 id = 0; QString name; QString sku; qint64 priceCents = 0; int stock = 0; int lowStockThreshold = 5; };
struct CartLine { Product product; int quantity = 1; int discountCents = 0; qint64 subtotalCents() const { return product.priceCents * quantity - discountCents; } };
struct SaleTotals { qint64 subtotalCents = 0; qint64 discountCents = 0; qint64 taxCents = 0; qint64 totalCents = 0; };
