QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TEMPLATE = app

DESTDIR = bin
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
unix:OBJECTS_DIR = build/o/unix
win32:OBJECTS_DIR = build/o/win32
macx:OBJECTS_DIR = build/o/mac

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mapmanager.cpp \
    mapscene.cpp \
    mapview.cpp
 #    mapview.cpp

HEADERS += \
    mainwindow.h \
    mapmanager.h \
    mapscene.h \
    mapview.h
 #    mapview.h

FORMS += \
    mainwindow.ui \
    mapmanager.ui \
    mapview.ui
 #    mapview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
