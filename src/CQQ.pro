#-------------------------------------------------
#
# Project created by QtCreator 2019-02-12T18:53:49
#
#-------------------------------------------------

QT       += core gui websockets webenginewidgets network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CQQ
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
    fetchfriends.cpp \
        main.cpp \
        mainwindow.cpp \
    unifiedicon.cpp \
    wsconnection.cpp \
    cachemanager.cpp \
    chatmanager.cpp \
    cqcode.cpp \
    configmanager.cpp \
    downloadmanager.cpp \
    messageedit.cpp \
    messagebrowser.cpp \
    chatlist.cpp \
    infowidget.cpp \
    contactlist.cpp \
    messageedittool.cpp \
    facedialog.cpp

HEADERS += \
    fetchfriends.h \
        mainwindow.h \
    unifiedicon.h \
    wsconnection.h \
    cachemanager.h \
    chatmanager.h \
    cqcode.h \
    configmanager.h \
    downloadmanager.h \
    messageedit.h \
    messagebrowser.h \
    chatlist.h \
    infowidget.h \
    contactlist.h \
    messageedittool.h \
    facedialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    .gitignore
