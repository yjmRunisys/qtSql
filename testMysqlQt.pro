#-------------------------------------------------
#
# Project created by QtCreator 2018-03-26T11:55:35
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testMysqlQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    brower.cpp \
    connectionwidget.cpp \


HEADERS  += mainwindow.h \
    brower.h \
    config.h \
    connectionwidget.h \


FORMS    += mainwindow.ui \
    brower.ui \   

