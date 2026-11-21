#include <QtTest>
#include <QDir>
#include <QRandomGenerator>
#include <QtSql/QSqlQuery>
#include "database.h"
#include "services/productservice.h"
#include "services/saleservice.h"
class CoreTests : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() { QString error; QVERIFY2(Database::initialize(&error, QDir::tempPath()+"/nova_pos_tests.sqlite"), qPrintable(error)); QSqlQuery q(Database::connection()); QVERIFY(q.exec("INSERT OR IGNORE INTO users(id,username,password_hash,salt,role) VALUES(1,'test','x','x','admin')")); }
    void totals() { Product p{1,"Coffee","COF",350,5,2}; auto t=SalesService::calculate({{p,2,50}}, 1000); QCOMPARE(t.subtotalCents,700); QCOMPARE(t.discountCents,50); QCOMPARE(t.taxCents,65); QCOMPARE(t.totalCents,715); }
    void stockAndRefund() { QString e; QVERIFY2(ProductService::save({0,"Test Item","TEST-"+QString::number(QRandomGenerator::global()->generate()),100,3,1},&e),qPrintable(e)); auto p=ProductService::search("Test Item").last(); auto id=SalesService::checkout({{p,2,0}},0,200,1,&e); QVERIFY2(id>0,qPrintable(e)); QCOMPARE(ProductService::search(p.sku).first().stock,1); QVERIFY2(SalesService::refund(id,1,&e),qPrintable(e)); QCOMPARE(ProductService::search(p.sku).first().stock,3); }
};
QTEST_MAIN(CoreTests)
#include "core_tests.moc"
