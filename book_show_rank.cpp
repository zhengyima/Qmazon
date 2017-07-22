#include "book_show_rank.h"
#include "ui_book_show_rank.h"

book_show_rank::book_show_rank(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::book_show_rank)
{
    ui->setupUi(this);
}

book_show_rank::~book_show_rank()
{
    delete ui;
}
