#ifndef BOOK_ADD_H
#define BOOK_ADD_H

#include <QWidget>

namespace Ui {
class book_add;
}

class book_add : public QWidget
{
    Q_OBJECT

public:
    explicit book_add(QWidget *parent = 0);
    ~book_add();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::book_add *ui;
};

#endif // BOOK_ADD_H
