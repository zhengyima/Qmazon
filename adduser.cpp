#include "adduser.h"
#include "ui_adduser.h"

adduser::adduser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adduser)
{
    ui->setupUi(this);
}

adduser::~adduser()
{
    delete ui;
}
