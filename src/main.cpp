#include "database.h"
#include "mainwindow.h"
#include "services/authservice.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("POS System");
    application.setOrganizationName("POS System");

    QFile styleFile(":/assets/styles/dark.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text))
        application.setStyleSheet(QString::fromUtf8(styleFile.readAll()));

    QString databaseError;
    if (!Database::initialize(&databaseError)) {
        QMessageBox::critical(nullptr, "Database error",
                              "The local product database could not be initialized.\n\n" + databaseError);
        return 1;
    }
    AuthService::ensureDefaultAdmin();
    QDialog login;
    login.setWindowTitle("Sign in — POS System"); login.setMinimumWidth(360);
    auto *form = new QFormLayout(&login); auto *username = new QLineEdit("admin"); auto *password = new QLineEdit; password->setEchoMode(QLineEdit::Password);
    form->addRow("Username", username); form->addRow("Password", password); auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel); buttons->button(QDialogButtonBox::Ok)->setText("Sign in"); form->addRow(buttons);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &login, &QDialog::accept); QObject::connect(buttons, &QDialogButtonBox::rejected, &login, &QDialog::reject);
    UserSession session;
    while (!session.id) { if (login.exec() != QDialog::Accepted) return 0; QString error; session = AuthService::login(username->text(), password->text(), &error); if (!session.id) QMessageBox::warning(&login, "Sign in failed", error); }
    MainWindow window(session);
    window.show();
    return application.exec();
}
