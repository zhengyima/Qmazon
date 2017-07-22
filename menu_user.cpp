#include "menu_user.h"
#include "ui_menu_user.h"

menu_user::menu_user(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu_user)
{
    ui->setupUi(this);
}

menu_user::~menu_user()
{
    delete ui;
}

