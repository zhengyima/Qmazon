#include "book_change.h"
#include "ui_book_change.h"

book_change::book_change(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::book_change)
{
    ui->setupUi(this);
}

book_change::~book_change()
{
    delete ui;
}
