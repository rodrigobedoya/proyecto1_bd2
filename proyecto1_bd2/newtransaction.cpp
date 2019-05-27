#include "newtransaction.h"
#include "ui_newtransaction.h"
#include "makequery.h"
#include "fstream"
#include <QFile>
#include <QTextStream>
#include <string>

NewTransaction::NewTransaction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTransaction)
{
    ui->setupUi(this);
    std::ofstream tiFile;
    //erase previous contents of instructions.txt
    tiFile.open("../proyecto1_bd2/instructions.txt", std::ofstream::out | std::ofstream::trunc);
    tiFile.close();
}

NewTransaction::~NewTransaction()
{
    delete ui;
}

void NewTransaction::on_pushButton_clicked()
{
    makeQuery newQuery(this);
    newQuery.setWindowTitle("New Query");
    int queryState = newQuery.exec();
    if(queryState == QDialog::Rejected)
        return;
    QString qstrQuery = newQuery.getQuery(); //get user's instruction input
    std::ofstream tiFile;

    std::string stdQuery = qstrQuery.toStdString();

    Request request;
    if(request.processQuery(stdQuery) == 0) //if request is parsed correctly
    {
        transaction.addRequest(request);
        std::cout << "added instruction #"<<transaction.numInstructions()<<std::endl;
        //add isntruction in instructions.txt
        tiFile.open("../proyecto1_bd2/instructions.txt", std::ios_base::app);
        tiFile << stdQuery<<";\n";
        tiFile.close();

        //show all lines of instructions.txt (udpdate data showed in interface)
        QFile file("../proyecto1_bd2/instructions.txt");
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        QString content = stream.readAll();
        file.close();
        ui->textBrowser->setPlainText(content);
    }
}

void NewTransaction::on_buttonBox_accepted()
{
    accept();
}

void NewTransaction::on_buttonBox_rejected()
{
    reject();
}
