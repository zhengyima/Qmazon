#include "menu_admin.h"
#include "ui_menu_admin.h"

menu_admin::menu_admin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu_admin)
{
    ui->setupUi(this);
}

menu_admin::~menu_admin()
{
    delete ui;
}


