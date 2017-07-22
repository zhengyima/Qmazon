#ifndef MENU_USER_H
#define MENU_USER_H

#include <QWidget>

namespace Ui {
class menu_user;
}

class menu_user : public QWidget
{
    Q_OBJECT

public:
    explicit menu_user(QWidget *parent = 0);
    ~menu_user();
    Ui::menu_user *ui;
    void menu_user::closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();


    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
};

#endif // MENU_USER_H
