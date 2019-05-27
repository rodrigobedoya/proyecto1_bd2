#ifndef MAKEQUERY_H
#define MAKEQUERY_H

#include <QDialog>
#include <QString>

namespace Ui {
class makeQuery;
}

class makeQuery : public QDialog
{
    Q_OBJECT

public:
    explicit makeQuery(QWidget *parent = 0);
    ~makeQuery();
    QString getQuery();
    QString getIndexing();
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::makeQuery *ui;
};

#endif // MAKEQUERY_H
