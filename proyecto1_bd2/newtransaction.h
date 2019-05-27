#ifndef NEWTRANSACTION_H
#define NEWTRANSACTION_H

#include <QDialog>
#include <QTextStream>
#include "model/transaction.h"

namespace Ui {
class NewTransaction;
}

class NewTransaction : public QDialog
{
    Q_OBJECT

public:
    explicit NewTransaction(QWidget *parent = 0);
    ~NewTransaction();
    Transaction transaction;

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::NewTransaction *ui;
};

#endif // NEWTRANSACTION_H
