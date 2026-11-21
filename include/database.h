#pragma once

#include <QString>
#include <QtSql/QSqlDatabase>

class Database
{
public:
    static bool initialize(QString *errorMessage = nullptr, const QString &databasePath = {});
    static QSqlDatabase connection();
    static QString path();
};
