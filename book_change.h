#ifndef BOOK_CHANGE_H
#define BOOK_CHANGE_H

#include <QWidget>

namespace Ui {
class book_change;
}

class book_change : public QWidget
{
    Q_OBJECT

public:
    explicit book_change(QWidget *parent = 0);
    ~book_change();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::book_change *ui;
};

#endif // BOOK_CHANGE_H
