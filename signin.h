#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>

namespace Ui {
class signin;
}

class signin : public QWidget
{
    Q_OBJECT

public:
    explicit signin(QWidget *parent = 0);
    ~signin();
    Ui::signin *ui;
    //void signin::closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_radioButton_clicked();

private:

};

#endif // SIGNIN_H
