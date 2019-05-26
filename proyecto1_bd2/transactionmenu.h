#ifndef TRANSACTIONMENU_H
#define TRANSACTIONMENU_H

#include <QDialog>
#include "model/transaction_handler.h"

namespace Ui {
class TransactionMenu;
}

class TransactionMenu : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionMenu(QWidget *parent = 0);
    ~TransactionMenu();
    TransactionHandler handler;

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::TransactionMenu *ui;
};

#endif // TRANSACTIONMENU_H
