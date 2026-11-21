#pragma once
#include <QString>
struct UserSession { qint64 id = 0; QString username; QString role; bool isAdmin() const { return role == "admin"; } };
class AuthService { public: static UserSession login(const QString &, const QString &, QString * = nullptr); static bool createUser(const QString &, const QString &, const QString &, QString * = nullptr); static void ensureDefaultAdmin(); };
