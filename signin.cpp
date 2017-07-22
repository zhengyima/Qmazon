#include "signin.h"
#include "ui_signin.h"
#include <QFile>
signin::signin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signin)
{
    ui->setupUi(this);


}

signin::~signin()
{
    delete ui;
}
