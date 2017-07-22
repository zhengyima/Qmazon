#ifndef USER_CHANGE_H
#define USER_CHANGE_H

#include <QWidget>

namespace Ui {
class user_change;
}

class user_change : public QWidget
{
    Q_OBJECT

public:
    explicit user_change(QWidget *parent = 0);
    ~user_change();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::user_change *ui;
};

#endif // USER_CHANGE_H
