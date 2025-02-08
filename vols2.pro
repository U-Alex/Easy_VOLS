QT      += core gui
QT      += network

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
    b_logic/restaccessmanager.cpp \
    b_logic/usersession.cpp \
    main.cpp \
    mainwindow.cpp \
    map/mapmanager.cpp \
    map/mapscene.cpp \
    map/mapview.cpp \
    map/obj/objcoup.cpp \
    map/obj/objfab.cpp \
    map/obj/objlocker.cpp \
    map/obj/objpwcont.cpp

HEADERS += \
    b_logic/restaccessmanager.h \
    b_logic/usersession.h \
    mainwindow.h \
    map/mapmanager.h \
    map/mapscene.h \
    map/mapview.h \
    map/obj/obj.h \
    map/obj/objcoup.h \
    map/obj/objfab.h \
    map/obj/objlocker.h \
    map/obj/objpwcont.h

FORMS += \
    ui/mainwindow.ui \
    ui/mapmanager.ui \
    ui/mapview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
