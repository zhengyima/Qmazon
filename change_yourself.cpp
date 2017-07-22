#include "change_yourself.h"
#include "ui_change_yourself.h"

change_yourself::change_yourself(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::change_yourself)
{
    ui->setupUi(this);
}

change_yourself::~change_yourself()
{
    delete ui;
}


