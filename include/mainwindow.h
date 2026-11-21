#pragma once
#include <QMainWindow>
#include "models/types.h"
#include "services/authservice.h"

class QLineEdit; class QStackedWidget; class QTableWidget; class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(UserSession session, QWidget *parent = nullptr);

private slots:
    void refreshAll();
    void addProductToCart();
    void checkout();
    void saveProduct();
    void deleteProduct();
    void adjustInventory();
    void refundSale();
    void exportReport();
    void saveSettings();
    void createUser();

private:
    QWidget *makeDashboard(); QWidget *makeCashier(); QWidget *makeProducts(); QWidget *makeInventory(); QWidget *makeSales(); QWidget *makeReports(); QWidget *makeUsers(); QWidget *makeSettings();
    void refreshProducts(); void refreshCart(); void refreshSales(); void refreshDashboard();
    UserSession session_; QVector<CartLine> cart_;
    QStackedWidget *pages_ = nullptr; QTableWidget *productTable_ = nullptr; QTableWidget *cashierProducts_ = nullptr; QTableWidget *cartTable_ = nullptr; QTableWidget *salesTable_ = nullptr; QTableWidget *inventoryTable_ = nullptr; QTableWidget *usersTable_ = nullptr;
    QLineEdit *productSearch_ = nullptr; QLineEdit *cashierSearch_ = nullptr; QLabel *cartTotal_ = nullptr; QLabel *dashboardStats_ = nullptr; QLineEdit *storeName_ = nullptr; QLineEdit *taxRate_ = nullptr;
};
