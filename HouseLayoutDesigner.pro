QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addcommand.cpp \
    command.cpp \
    deletecommand.cpp \
    furniture.cpp \
    housescene.cpp \
    main.cpp \
    mainwindow.cpp \
    movecommand.cpp \
    wall.cpp

HEADERS += \
    addcommand.h \
    command.h \
    deletecommand.h \
    furniture.h \
    housescene.h \
    mainwindow.h \
    movecommand.h \
    wall.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
