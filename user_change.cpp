#include "user_change.h"
#include "ui_user_change.h"

user_change::user_change(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::user_change)
{
    ui->setupUi(this);
}

user_change::~user_change()
{
    delete ui;
}
