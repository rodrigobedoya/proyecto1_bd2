#include "makequery.h"
#include "ui_makequery.h"
#include <QDebug>

makeQuery::makeQuery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::makeQuery)
{
    ui->setupUi(this);
}

makeQuery::~makeQuery()
{
    delete ui;
}

void makeQuery::on_buttonBox_accepted()
{
    accept();
}

void makeQuery::on_buttonBox_rejected()
{
    reject();
}

QString makeQuery::getQuery()
{
    return ui->query->text(); //get user's query input as string 
}

QString makeQuery::getIndexing()
{
    QString str = ui->comboBox->currentText(); //get indexing type chosen by user
    return str;
}
