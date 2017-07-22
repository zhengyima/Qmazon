#include "add_users.h"
#include "ui_add_users.h"

add_users::add_users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_users)
{
    ui->setupUi(this);
}

add_users::~add_users()
{
    delete ui;
}
