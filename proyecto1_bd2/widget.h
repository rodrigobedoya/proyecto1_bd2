#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "file_organization/randomfile.h"
#include "model/artist.h"
#include "model/release.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    RandomFile randomFile;
    void releaseQuery(vector<Release*> releases);
    void artistQuery(vector<Artist*> artists);
};

#endif // WIDGET_H
