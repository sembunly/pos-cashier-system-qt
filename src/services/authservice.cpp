#include "services/authservice.h"
#include "database.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

namespace {
QString digest(const QString &password, const QString &salt) {
    QByteArray value = salt.toUtf8() + password.toUtf8();
    for (int i = 0; i < 120000; ++i) value = QCryptographicHash::hash(value + salt.toUtf8(), QCryptographicHash::Sha256);
    return value.toHex();
}
}
bool AuthService::createUser(const QString &username, const QString &password, const QString &role, QString *error) {
    if (username.trimmed().isEmpty() || password.isEmpty() || (role != "admin" && role != "cashier")) { if(error) *error="Username and password are required."; return false; }
    const QString salt = QString::number(QRandomGenerator::global()->generate64(), 16);
    QSqlQuery q(Database::connection()); q.prepare("INSERT INTO users(username,password_hash,salt,role) VALUES(?,?,?,?)"); q.addBindValue(username.trimmed()); q.addBindValue(digest(password,salt)); q.addBindValue(salt); q.addBindValue(role);
    if (!q.exec()) { if(error) *error=q.lastError().text(); return false; } return true;
}
void AuthService::ensureDefaultAdmin() { QSqlQuery q(Database::connection()); q.exec("SELECT COUNT(*) FROM users"); if (q.next() && q.value(0).toInt()==0) { createUser("admin", "1", "admin"); return; } QSqlQuery current(Database::connection()); current.prepare("SELECT salt FROM users WHERE username='admin'"); if (current.exec() && current.next()) { QSqlQuery update(Database::connection()); update.prepare("UPDATE users SET password_hash=? WHERE username='admin'"); update.addBindValue(digest("1", current.value(0).toString())); update.exec(); } }
UserSession AuthService::login(const QString &username, const QString &password, QString *error) {
    QSqlQuery q(Database::connection()); q.prepare("SELECT id,username,password_hash,salt,role FROM users WHERE username=? AND is_active=1"); q.addBindValue(username.trimmed());
    if (!q.exec() || !q.next() || digest(password,q.value(3).toString()) != q.value(2).toString()) { if(error) *error="Invalid username or password."; return {}; }
    return {q.value(0).toLongLong(),q.value(1).toString(),q.value(4).toString()};
}
