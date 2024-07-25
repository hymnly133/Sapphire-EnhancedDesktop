QT += core gui widgets winextras network concurrent quick qml

VERSION = 1.2.8.3
msvc{
    message("using msvc")
    QMAKE_CXXFLAGS += -execution-charset:utf-8
    QMAKE_CXXFLAGS += -source-charset:utf-8

    # CONFIG(debug, debug|release) {
    # message("debug");
    #     LIBS += -L$$PWD/lib/ -lelawidgettoolsd
    # } else {
    # message("release");
    #     LIBS += -L$$PWD/lib/ -lelawidgettools
    # }
}

mingw{
message("using mingw")

# LIBS += -L$$PWD/lib/ -lelawidgettools

}
QMAKE_LIBDIR += $$PWD/lib
INCLUDEPATH+= $$PWD/include
INCLUDEPATH+= $$PWD/include/ContextMenu
# INCLUDEPATH+= $$PWD/include/Ela
# INCLUDEPATH+= $$PWD/include/Ela/Example
# DEPENDPATH += $$PWD/include/Ela

LIBS+= -luser32    # 使用WindowsAPI需要链接库
LIBS+= -ldwmapi
LIBS += -lOle32  -luuid -lshlwapi -luser32 -lshell32 -lsetupapi -lcomctl32



QMAKE_PROJECT_DEPTH = 0




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


SOURCES += SysFunctions.cpp \
    filefunc.cpp \
    include/layerbackground.cpp \
    include/picturebox.cpp \
    include/repaintcounterunit.cpp \
    include/roundshower.cpp \
    include/sanimationrect.cpp \
    include/sbgshower.cpp \
    include/sblockcontainer.cpp \
    include/sblocklayout.cpp \
    include/scontainer.cpp \
    include/sdock.cpp \
    include/seditbox.cpp \
    include/sfile.cpp \
    include/shidetextblock.cpp \
    include/slayout.cpp \
    include/slinearlayout.cpp \
    include/smultifunc.cpp \
    include/snotice.cpp \
    include/sshellfuncunit.cpp \
    include/stooltip.cpp \
    include/sunit.cpp \
    layershower.cpp \
    main.cpp \
    mainwindow.cpp \
    screenfunc.cpp \
    style.cpp\
    include/ContextMenu/contextmenu.cpp \
    include/ContextMenu/desktopmenu.cpp \
    include/ContextMenu/shellitem.cpp \
    include/ContextMenu/shellmemmanager.cpp \
    # include/settingwindow.cpp\

HEADERS += \
    include/ContextMenu/contextmenu.h \
    include/ContextMenu/desktopmenu.h \
    include/ContextMenu/shellitem.h \
    include/ContextMenu/shellmemmanager.h \
    include/ContextMenu/shellmenuitem.h \
    include/SysFunctions.h \
    include/filefunc.h \
    include/layerbackground.h \
    include/layershower.h \
    include/mainwindow.h \
    include/picturebox.h \
    include/repaintcounterunit.h \
    include/roundshower.h \
    include/sanimationrect.h \
    include/sbgshower.h \
    include/sblockcontainer.h \
    include/sblocklayout.h \
    include/scontainer.h \
    include/screenfunc.h \
    include/sdock.h \
    include/seditbox.h \
    # include/settingwindow.h \
    include/sfile.h \
    include/shidetextblock.h \
    include/slayout.h \
    include/slinearlayout.h \
    include/smultifunc.h \
    include/snotice.h \
    include/sshellfuncunit.h \
    include/stooltip.h \
    include/style.h \
    include/sunit.h


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
    include/Ela/ElaWidgetTools.qrc \
    res.qrc

DISTFILES += \
    README.md \
    Sapphire.rep \
    SysIcons/imageres 023.png \
    SysIcons/imageres 050.png \
    SysIcons/imageres 051.png \
    SysIcons/imageres 105.png \
    style.json












# INCLUDEPATH += $$PWD/include/Ela
# DEPENDPATH += $$PWD/include/Ela
