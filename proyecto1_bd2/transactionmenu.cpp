#include "transactionmenu.h"
#include "ui_transactionmenu.h"
#include "newtransaction.h"
#include <fstream>
#include <QFile>

TransactionMenu::TransactionMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionMenu)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Run");
    std::ofstream tFile;
    //erase content of transactions.txt
    tFile.open("../proyecto1_bd2/transactions.txt", std::ofstream::out | std::ofstream::trunc); 
    tFile.close();
}

TransactionMenu::~TransactionMenu()
{
    delete ui;
}

void TransactionMenu::on_pushButton_clicked()
{
    std::cout << "Beginning new transaction"<<std::endl;
    NewTransaction newTransaction(this);
    newTransaction.setWindowTitle("New Transaction");
    int newTransactionState = newTransaction.exec();
    if(newTransactionState == QDialog::Rejected)
    {
        std::cout << "Transaction deleted"<<std::endl;
        return;
    }
    handler.add(newTransaction.transaction);
    std::cout << "Transaction saved to transaction handler"<<std::endl;

    //get new transaction instructions
    QFile file("../proyecto1_bd2/instructions.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString content = "Transaction ";
    content += QString::number(handler.size())+":\n";
    content += stream.readAll();
    content += "\n";


    //add new transaction instructions to transactions.txt
    QFile file2("../proyecto1_bd2/transactions.txt");
    file2.open(QIODevice::WriteOnly| QIODevice::Append);
    QTextStream stream2(&file2);
    stream2 << content;
    file2.close();

    //show all lines in transactions.txt(update interface)
    file2.open(QIODevice::ReadOnly);
    QTextStream stream3(&file2);
    QString content2 = stream3.readAll();
    file2.close();
    ui->textBrowser->setPlainText(content2);

}

void TransactionMenu::on_buttonBox_accepted()
{
    accept();
}

void TransactionMenu::on_buttonBox_rejected()
{
    reject();
}
