#ifndef LOADING_H
#define LOADING_H

#include <QWidget>

namespace Ui {
class Loading;
}

class Loading : public QWidget
{
    Q_OBJECT

public:
    explicit Loading(QWidget *parent = 0);
    ~Loading();
    Ui::Loading *ui;

};

#endif // LOADING_H
