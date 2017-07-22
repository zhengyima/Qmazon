#ifndef ADDUSER_H
#define ADDUSER_H

#include <QWidget>

namespace Ui {
class adduser;
}

class adduser : public QWidget
{
    Q_OBJECT

public:
    explicit adduser(QWidget *parent = 0);
    ~adduser();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::adduser *ui;
};

#endif // ADDUSER_H
