#include "services/productservice.h"
#include "database.h"
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
QVector<Product> ProductService::search(const QString &term) {
    QVector<Product> out; QSqlQuery q(Database::connection()); q.prepare("SELECT id,name,sku,price_cents,stock_quantity,low_stock_threshold FROM products WHERE is_active=1 AND (name LIKE ? OR sku LIKE ?) ORDER BY name"); const QString like="%"+term.trimmed()+"%"; q.addBindValue(like); q.addBindValue(like); if(q.exec()) while(q.next()) out.push_back({q.value(0).toLongLong(),q.value(1).toString(),q.value(2).toString(),q.value(3).toLongLong(),q.value(4).toInt(),q.value(5).toInt()}); return out;
}
bool ProductService::save(Product p, QString *error) {
    if(p.name.trimmed().isEmpty() || p.sku.trimmed().isEmpty() || p.priceCents<0 || p.stock<0) {if(error)*error="Name, SKU, non-negative price and stock are required.";return false;} QSqlQuery q(Database::connection());
    if(p.id) q.prepare("UPDATE products SET name=?,sku=?,price_cents=?,stock_quantity=?,low_stock_threshold=?,updated_at=CURRENT_TIMESTAMP WHERE id=?"); else q.prepare("INSERT INTO products(name,sku,price_cents,stock_quantity,low_stock_threshold) VALUES(?,?,?,?,?)");
    q.addBindValue(p.name.trimmed());q.addBindValue(p.sku.trimmed());q.addBindValue(p.priceCents);q.addBindValue(p.stock);q.addBindValue(p.lowStockThreshold);if(p.id)q.addBindValue(p.id); if(!q.exec()){if(error)*error=q.lastError().text();return false;}return true;
}
bool ProductService::softDelete(qint64 id, QString *error) { QSqlQuery q(Database::connection());q.prepare("UPDATE products SET is_active=0 WHERE id=?");q.addBindValue(id);if(!q.exec()){if(error)*error=q.lastError().text();return false;}return true; }
bool ProductService::adjustStock(qint64 id,int delta,const QString &reason,qint64 userId,QString *error) { auto db=Database::connection(); if(!db.transaction())return false; QSqlQuery update(db);update.prepare("UPDATE products SET stock_quantity=stock_quantity+?, updated_at=CURRENT_TIMESTAMP WHERE id=? AND stock_quantity+?>=0");update.addBindValue(delta);update.addBindValue(id);update.addBindValue(delta);if(!update.exec()||update.numRowsAffected()!=1){db.rollback();if(error)*error="Adjustment would make stock negative.";return false;} QSqlQuery log(db);log.prepare("INSERT INTO inventory_transactions(product_id,delta,reason,user_id) VALUES(?,?,?,?)");log.addBindValue(id);log.addBindValue(delta);log.addBindValue(reason);log.addBindValue(userId);if(!log.exec()){db.rollback();if(error)*error=log.lastError().text();return false;}return db.commit(); }
