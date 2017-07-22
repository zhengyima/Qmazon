#ifndef ADD_USERS_H
#define ADD_USERS_H

#include <QWidget>

namespace Ui {
class add_users;
}

class add_users : public QWidget
{
    Q_OBJECT

public:
    explicit add_users(QWidget *parent = 0);
    ~add_users();

private:
    Ui::add_users *ui;
};

#endif // ADD_USERS_H
