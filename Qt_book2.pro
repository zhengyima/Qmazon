#-------------------------------------------------
#
# Project created by QtCreator 2016-12-18T22:52:57
#
#-------------------------------------------------

QT       += core gui
QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_book2
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    change_yourself.cpp \
    menu_user.cpp \
    signup.cpp \
    signin.cpp \
    book_add.cpp \
    book_change.cpp \
    menu_admin.cpp \
    user_change.cpp \
    adduser.cpp

HEADERS  += widget.h \
    base.h \
    change_yourself.h \
    signin.h \
    signup.h \
    menu_user.h \
    book_add.h \
    book_change.h \
    menu_admin.h \
    user_change.h \
    adduser.h

FORMS    += widget.ui \
    change_yourself.ui \
    menu_user.ui \
    signin.ui \
    signup.ui \
    book_add.ui \
    book_change.ui \
    menu_admin.ui \
    user_change.ui \
    adduser.ui

DISTFILES += \
    BX-Book-Ratings.csv \
    BX-Books.csv \
    BX-Users.csv \
    myico.ico

RESOURCES += \
    myresource.qrc
RC_FILE += myico.rc
