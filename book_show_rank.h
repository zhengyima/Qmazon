#ifndef BOOK_SHOW_RANK_H
#define BOOK_SHOW_RANK_H

#include <QWidget>

namespace Ui {
class book_show_rank;
}

class book_show_rank : public QWidget
{
    Q_OBJECT

public:
    explicit book_show_rank(QWidget *parent = 0);
    ~book_show_rank();

private:
    Ui::book_show_rank *ui;
};

#endif // BOOK_SHOW_RANK_H
