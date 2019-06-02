#-------------------------------------------------
#
# Project created by QtCreator 2019-03-30T19:25:26
#
#-------------------------------------------------

QT       += core gui sql charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = server
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../share/pipe.cpp \
    server.cpp \
    manager.cpp \
    managerwindow.cpp \
    managercontrol.cpp \
    records.cpp \
    recordscontrol.cpp \
    recordswindow.cpp \
    simplebillwindow.cpp \
    reportformwindow.cpp

HEADERS += \
        mainwindow.h \
    ../share/objects.h \
    ../share/pipe.h \
    server.h \
    manager.h \
    managerwindow.h \
    managercontrol.h \
    records.h \
    recordscontrol.h \
    recordswindow.h \
    simplebillwindow.h \
    reportformwindow.h

FORMS += \
        mainwindow.ui \
    managerwindow.ui \
    recordswindow.ui \
    simplebillwindow.ui \
    reportformwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
