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
    return ui->query->text();
}

QString makeQuery::getIndexing()
{
    QString str = ui->comboBox->currentText();
    return str;
}
