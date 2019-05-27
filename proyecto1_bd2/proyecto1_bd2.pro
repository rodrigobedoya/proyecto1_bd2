#-------------------------------------------------
#
# Project created by QtCreator 2019-05-22T12:22:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = proyecto1_bd2
TEMPLATE = app

CONFIG += c++14
LIBS += -pthread
SOURCES += main.cpp\
        widget.cpp \
    makequery.cpp \
    transactionmenu.cpp \
    newtransaction.cpp

HEADERS  += widget.h \
    makequery.h \
    file_organization/randomfile.h \
    model/artist.h \
    model/release.h \
    model/request.h \
    transactionmenu.h \
    newtransaction.h \
    model/transaction.h \
    model/transaction_handler.h \
    file_organization/bucket.h \
    file_organization/directory.h

FORMS    += widget.ui \
    makequery.ui \
    transactionmenu.ui \
    newtransaction.ui

DISTFILES += \
    randomFileReleases.dat \
    releases.dat

RESOURCES += \
    resources.qrc
