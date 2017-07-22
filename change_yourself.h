#ifndef CHANGE_YOURSELF_H
#define CHANGE_YOURSELF_H

#include <QWidget>

namespace Ui {
class change_yourself;
}

class change_yourself : public QWidget
{
    Q_OBJECT

public:
    explicit change_yourself(QWidget *parent = 0);
    ~change_yourself();
     Ui::change_yourself *ui;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

   // void on_pushButton_2_clicked();

private:

};

#endif // CHANGE_YOURSELF_H
