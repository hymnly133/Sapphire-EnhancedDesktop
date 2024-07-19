QT += core gui widgets winextras network concurrent quick qml
win32 {
LIBS+= -luser32    # 使用WindowsAPI需要链接库
LIBS+= -ldwmapi
LIBS += -lOle32  -luuid -lshlwapi -luser32 -lshell32 -lsetupapi
}
msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8

VERSION = 1.2.4.1
QMAKE_PROJECT_DEPTH = 0
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ContextMenu/contextmenu.cpp \
    ContextMenu/shellitem.cpp \
    ContextMenu/shellmemmanager.cpp \
    SysFunctions.cpp \
    ed_bgshower.cpp \
    ed_block.cpp \
    ed_blockcontainer.cpp \
    ed_blocklayout.cpp \
    ed_container.cpp \
    ed_dock.cpp \
    ed_editbox.cpp \
    ed_hidetextblock.cpp \
    ed_layout.cpp \
    ed_linearlayout.cpp \
    ed_unit.cpp \
    filepreviewwidget.cpp \
    hitokoto.cpp \
    layershower.cpp \
    main.cpp \
    mainwindow.cpp \
    mousehook.cpp \
    picturebox.cpp \
    roundshower.cpp \
    style.cpp \
    weather.cpp

HEADERS += \
    ContextMenu/contextmenu.h \
    ContextMenu/shellitem.h \
    ContextMenu/shellmemmanager.h \
    SysFunctions.h \
    ed_bgshower.h \
    ed_block.h \
    ed_blockcontainer.h \
    ed_blocklayout.h \
    ed_container.h \
    ed_dock.h \
    ed_editbox.h \
    ed_hidetextblock.h \
    ed_layout.h \
    ed_linearlayout.h \
    ed_unit.h \
    filepreviewwidget.h \
    hitokoto.h \
    layershower.h \
    mainwindow.h \
    mousehook.h \
    picturebox.h \
    roundshower.h \
    style.h \
    weather.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Sapphire_zh_CN.ts \
CONFIG += lrelease \
CONFIG += embed_translations \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    res.qrc

DISTFILES += \
    README.md \
    style.json




