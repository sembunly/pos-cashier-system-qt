#pragma once
#include "models/types.h"
class SalesService { public: static SaleTotals calculate(const QVector<CartLine> &, int taxBasisPoints); static qint64 checkout(const QVector<CartLine> &, int taxBasisPoints, qint64 cashTenderedCents, qint64 userId, QString *error = nullptr); static bool refund(qint64 saleId, qint64 userId, QString *error = nullptr); };
