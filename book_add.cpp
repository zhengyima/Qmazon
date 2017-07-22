#include "book_add.h"
#include "ui_book_add.h"

book_add::book_add(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::book_add)
{
    ui->setupUi(this);
}

book_add::~book_add()
{
    delete ui;
}
