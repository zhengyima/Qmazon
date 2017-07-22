#ifndef MENU_ADMIN_H
#define MENU_ADMIN_H

#include <QWidget>

namespace Ui {
class menu_admin;
}

class menu_admin : public QWidget
{
    Q_OBJECT

public:
    explicit menu_admin(QWidget *parent = 0);
    ~menu_admin();
    void menu_admin::closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();


private:
    Ui::menu_admin *ui;
};

#endif // MENU_ADMIN_H
