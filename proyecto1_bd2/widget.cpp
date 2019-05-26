#include "widget.h"
#include "ui_widget.h"
#include "makequery.h"
#include "QString"
#include "unistd.h"
#include "transactionmenu.h"
#include <chrono>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Query");
    randomFile.create();
    randomFile.load();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_2_clicked()
{
    TransactionMenu transactionMenu(this);
    transactionMenu.setWindowTitle("Transactions Handler");
    int transactionMenuState = transactionMenu.exec();
    if(transactionMenuState == QDialog::Rejected)
        return;
    std::cout << "Running transactions"<<std::endl;
    transactionMenu.handler.run_all(randomFile);
}


void Widget::on_pushButton_clicked()
{
    makeQuery newQuery(this);
    newQuery.setWindowTitle("New Query");
    int queryState = newQuery.exec();
    if(queryState == QDialog::Rejected)
        return;

    QString qstrQuery = newQuery.getQuery();
    Request request;
    request.processQuery(qstrQuery.toStdString());
    std::cout << qstrQuery.toStdString() << std::endl;
    request.print();
    //QString table = "releases";
    //QString id = "1";

    QString indexing = newQuery.getIndexing();
    if (request.getTable()=="releases")
    {
        auto start = std::chrono::high_resolution_clock::now();
        vector<Release*> releases = randomFile.searchRelease(request.getVal(),request.getVarName(),request.getType(),indexing.toStdString());
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        cout << "time "  << elapsed.count() << " seconds"<<endl<<endl;
        releaseQuery(releases);
    }
    else if (request.getTable() == "artists")
    {
        auto start = std::chrono::high_resolution_clock::now();
        vector<Artist*> artists = randomFile.searchArtist(request.getVal(),request.getVarName(),request.getType(),indexing.toStdString());
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        cout << "time: "  << elapsed.count() << " seconds"<<endl<<endl;
        artistQuery(artists);
    }
}

void Widget::releaseQuery(vector<Release*> releases)
{
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(releases.size());
    QStringList headers;
    headers << "id" << "name" << "year" << "country" << "artistId";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (unsigned int row = 0; row < releases.size(); ++row)
    {
        QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item0->setData(Qt::DisplayRole, (unsigned long long)releases[row]->getId());
        ui->tableWidget->setItem(row, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem;
        item1->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        QString qstr = QString::fromStdString(releases[row]->getName());
        item1->setData(Qt::DisplayRole, qstr);
        ui->tableWidget->setItem(row, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem;
        item2->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item2->setData(Qt::DisplayRole, releases[row]->getYear());
        ui->tableWidget->setItem(row, 2, item2);

        QTableWidgetItem *item3 = new QTableWidgetItem;
        item3->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        QString qstr2 = QString::fromStdString(releases[row]->getCountry());
        item3->setData(Qt::DisplayRole, qstr2);
        ui->tableWidget->setItem(row, 3, item3);

        QTableWidgetItem *item4 = new QTableWidgetItem;
        item4->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item4->setData(Qt::DisplayRole, (long long)releases[row]->getArtistId());
        ui->tableWidget->setItem(row, 4, item4);
    }
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->resizeColumnsToContents();
}

void Widget::artistQuery(vector<Artist*> artists)
{
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(artists.size());
    QStringList headers;
    headers << "id" << "name";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (unsigned int row = 0; row < artists.size(); ++row)
    {
        QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        item0->setData(Qt::DisplayRole, (long long)artists[row]->getId());
        ui->tableWidget->setItem(row, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem;
        item1->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        QString qstr = QString::fromStdString(artists[row]->getName());
        item1->setData(Qt::DisplayRole, qstr);
        ui->tableWidget->setItem(row, 1, item1);
    }
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->resizeColumnsToContents();
}

