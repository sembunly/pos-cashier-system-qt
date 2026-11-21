#include "database.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

namespace {
constexpr auto kConnectionName = "pos_connection";
}

QSqlDatabase Database::connection() { return QSqlDatabase::database(kConnectionName); }

bool Database::initialize(QString *errorMessage, const QString &databasePath)
{
    QSqlDatabase db = QSqlDatabase::contains(kConnectionName)
        ? QSqlDatabase::database(kConnectionName)
        : QSqlDatabase::addDatabase("QSQLITE", kConnectionName);

    const QString dataDirectory = databasePath.isEmpty()
#ifdef POS_PROJECT_DATABASE_DIR
        ? QStringLiteral(POS_PROJECT_DATABASE_DIR)
#else
        ? QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
#endif
        : QFileInfo(databasePath).absolutePath();
    if (!QDir().mkpath(dataDirectory)) {
        if (errorMessage) *errorMessage = "Could not create the application data folder.";
        return false;
    }

    db.setDatabaseName(databasePath.isEmpty() ? dataDirectory + "/pos_cashier.sqlite" : databasePath);
    if (!db.open()) {
        if (errorMessage) *errorMessage = db.lastError().text();
        return false;
    }

    QSqlQuery pragma(db); pragma.exec("PRAGMA foreign_keys = ON");
    QFile schemaFile(":/database/schema.sql");
    if (!schemaFile.open(QFile::ReadOnly | QFile::Text)) { if (errorMessage) *errorMessage = "Unable to load database schema."; return false; }
    for (const QString &rawStatement : QString::fromUtf8(schemaFile.readAll()).split(';', Qt::SkipEmptyParts)) {
        const QString statement = rawStatement.trimmed();
        if (statement.isEmpty()) continue;
        QSqlQuery query(db);
        if (!query.exec(statement)) { if (errorMessage) *errorMessage = query.lastError().text(); return false; }
    }

    QSqlQuery columns(db); columns.exec("PRAGMA table_info(products)"); bool hasLowStock = false;
    while (columns.next()) hasLowStock = hasLowStock || columns.value(1).toString() == "low_stock_threshold";
    if (!hasLowStock) { QSqlQuery migration(db); if (!migration.exec("ALTER TABLE products ADD COLUMN low_stock_threshold INTEGER NOT NULL DEFAULT 5")) { if(errorMessage) *errorMessage=migration.lastError().text(); return false; } }
    return true;
}

QString Database::path() { return connection().databaseName(); }
