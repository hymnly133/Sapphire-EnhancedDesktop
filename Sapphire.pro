QT += core gui widgets winextras network concurrent qml xml opengl

QMAKE_CFLAGS_RELEASE += -g
QMAKE_CXXFLAGS_RELEASE += -g
#禁止优化
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_LFLAGS_RELEASE = -mthreads -W
# #生成dump文件需要
# LIBS += -lDbgHelp
#加上下面两行，否则用vs调试时，提示“未找到xxx.exe"
QMAKE_CXXFLAGS += -g
QMAKE_CFLAGS += -g


include($$PWD/include/QXlsx/QXlsx.pri)
INCLUDEPATH += $$PWD/QXlsx
msvc{
    message("using msvc")
    QMAKE_CXXFLAGS += -execution-charset:utf-8
    QMAKE_CXXFLAGS += -source-charset:utf-8

}
mingw{
message("using mingw")
RC_FILE=Sapphire.rc
# VERSION = 1.3.0.0
# LIBS += -L$$PWD/lib/ -lelawidgettools

}

win32:CONFIG(release, debug|release): {
LIBS += -L$$PWD/include/qBreakpad/lib/release/ -lqBreakpad
DEPENDPATH += $$PWD/include/qBreakpad/lib/release
}
else:win32:CONFIG(debug, debug|release): {
LIBS += -L$$PWD/include/qBreakpad/lib/debug/ -lqBreakpad
DEPENDPATH += $$PWD/include/qBreakpad/lib/debug
}


QMAKE_LIBDIR += $$PWD/lib
INCLUDEPATH+= $$PWD/include
INCLUDEPATH+= $$PWD
INCLUDEPATH+= $$PWD/include/ContextMenu
INCLUDEPATH+= $$PWD/include/SGUI
INCLUDEPATH+= $$PWD/include/SUnit
INCLUDEPATH+= $$PWD/include/SUnit/SContainer
INCLUDEPATH+= $$PWD/include/Func
INCLUDEPATH+= $$PWD/include/SLayer
INCLUDEPATH+= $$PWD/include/SLayout
INCLUDEPATH+= $$PWD/include/SObject
INCLUDEPATH+= $$PWD/include/SWidget
INCLUDEPATH+= $$PWD/include/Setting
INCLUDEPATH+= $$PWD/include/SInterface
INCLUDEPATH += $$PWD/include/qBreakpad


LIBS+= -luser32    # 使用WindowsAPI需要链接库
LIBS+= -ldwmapi
LIBS += -lOle32  -luuid -lshlwapi -lshell32 -lsetupapi -lcomctl32 -lopengl32 -lglu32
LIBS += -ltaskschd
LIBS += -loleaut32
LIBS += -lws2_32 -lwbemuuid



QMAKE_PROJECT_DEPTH = 0




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 thread exeptions rtti stl


SOURCES += \
    include/ContextMenu/contextmenu.cpp \
    include/ContextMenu/contextmenu1.cpp \
    include/ContextMenu/desktopmenu.cpp \
    include/ContextMenu/layertoppainter.cpp \
    include/ContextMenu/shellitem.cpp \
    include/ContextMenu/shellmemmanager.cpp \
    include/ContextMenu/sshellcontextmenu.cpp \
    include/Func/SysFunctions.cpp \
    include/Func/filefunc.cpp \
    include/Func/global.cpp \
    include/Func/guifunc.cpp \
    include/Func/iconfunc.cpp \
    include/Func/screenfunc.cpp \
    include/Func/smytaskschedule.cpp \
    include/Func/unitfunc.cpp \
    include/Func/userfunc.cpp \
    include/SGUI/saction.cpp \
    include/SGUI/scheckbox.cpp \
    include/SGUI/sinputdialog.cpp \
    include/SGUI/slabel.cpp \
    include/SGUI/smenu.cpp \
    include/SGUI/snotice.cpp \
    include/SGUI/sscrollarea.cpp \
    include/SGUI/stooltip.cpp \
    include/SGUI/strayicon.cpp \
    include/SInterface/sfileinfo.cpp \
    include/SInterface/slayoutcontainer.cpp \
    include/SInterface/spixtrueandname.cpp \
    include/SLayer/layerbackground.cpp \
    include/SLayer/layermousegetter.cpp \
    include/SLayer/layershower.cpp \
    include/SLayout/sblocklayout.cpp \
    include/SLayout/sflowlayout.cpp \
    include/SLayout/slayout.cpp \
    include/SLayout/slinearlayout.cpp \
    include/SObject/aerowidget.cpp \
    include/SObject/sanimationrect.cpp \
    include/SObject/sfieldswidget.cpp \
    include/SObject/sgifthread.cpp \
    include/SObject/slineranimation.cpp \
    include/SUnit/repaintcounterunit.cpp \
    include/SUnit/sblockcontainer.cpp \
    include/SUnit/scontainer.cpp \
    include/SUnit/sdir.cpp \
    include/SUnit/sdock.cpp \
    include/SUnit/seditbox.cpp \
    include/SUnit/sfile.cpp \
    include/SUnit/sglshower.cpp \
    include/SUnit/smultifunc.cpp \
    include/SUnit/sshellfuncunit.cpp \
    include/SUnit/sunit.cpp \
    include/SWidget/glpicturebox.cpp \
    include/SWidget/picturebox.cpp \
    include/SWidget/roundshower.cpp \
    include/SWidget/sbgshower.cpp \
    include/SWidget/scolorview.cpp \
    include/Setting/SQSS.cpp \
    include/Setting/sblocklayoutsettingwindow.cpp \
    include/Setting/style.cpp \
    include/Setting/stylehelper.cpp \
    include/Setting/stylesettingwindow.cpp \
    include/Setting/stylesettotal.cpp \
    include/Setting/sval.cpp \
    include/mainwindow.cpp \
    main.cpp


HEADERS += \
    include/ContextMenu/contextmenu.h \
    include/ContextMenu/contextmenu1.h \
    include/ContextMenu/desktopmenu.h \
    include/ContextMenu/layertoppainter.h \
    include/ContextMenu/shellitem.h \
    include/ContextMenu/shellmemmanager.h \
    include/ContextMenu/shellmenuitem.h \
    include/ContextMenu/sshellcontextmenu.h \
    include/Func/SysFunctions.h \
    include/Func/filefunc.h \
    include/Func/global.h \
    include/Func/guifunc.h \
    include/Func/iconfunc.h \
    include/Func/screenfunc.h \
    include/Func/smytaskschedule.h \
    include/Func/unitfunc.h \
    include/Func/userfunc.h \
    include/SGUI/saction.h \
    include/SGUI/scheckbox.h \
    include/SGUI/sinputdialog.h \
    include/SGUI/slabel.h \
    include/SGUI/smenu.h \
    include/SGUI/snotice.h \
    include/SGUI/sscrollarea.h \
    include/SGUI/stooltip.h \
    include/SGUI/strayicon.h \
    include/SInterface/sfileinfo.h \
    include/SInterface/slayoutcontainer.h \
    include/SInterface/spixtrueandname.h \
    include/SLayer/layerbackground.h \
    include/SLayer/layermousegetter.h \
    include/SLayer/layershower.h \
    include/SLayout/sblocklayout.h \
    include/SLayout/sflowlayout.h \
    include/SLayout/slayout.h \
    include/SLayout/slinearlayout.h \
    include/SObject/aerowidget.h \
    include/SObject/sanimationrect.h \
    include/SObject/sfieldswidget.h \
    include/SObject/sgifthread.h \
    include/SObject/slineranimation.h \
    include/SUnit/repaintcounterunit.h \
    include/SUnit/sblockcontainer.h \
    include/SUnit/scontainer.h \
    include/SUnit/sdir.h \
    include/SUnit/sdock.h \
    include/SUnit/seditbox.h \
    include/SUnit/sfile.h \
    include/SUnit/sglshower.h \
    include/SUnit/smultifunc.h \
    include/SUnit/sshellfuncunit.h \
    include/SUnit/sunit.h \
    include/SWidget/glpicturebox.h \
    include/SWidget/picturebox.h \
    include/SWidget/roundshower.h \
    include/SWidget/sbgshower.h \
    include/SWidget/scolorview.h \
    include/Setting/SQSS.h \
    include/Setting/sblocklayoutsettingwindow.h \
    include/Setting/style.h \
    include/Setting/stylehelper.h \
    include/Setting/stylesettingwindow.h \
    include/Setting/stylesettotal.h \
    include/Setting/sval.h \
    include/mainwindow.h \
    version.h


FORMS += \
    include/Setting/settingDialogForBlockLayout.ui \
    include/Setting/styleSetting.ui \
    include/Setting/stylesettotal.ui \
    include/colorvalwidgettest.ui \
    include/fieldWidget.ui \
    include/mainwindow.ui \
    include/scollTest.ui \
    include/widgeTest.ui

TRANSLATIONS += \
translation/Sapphire_en_US.ts \
translation/Sapphire_zh_CN.ts

CONFIG += lrelease \
CONFIG += embed_translations \



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    # include/Ela/ElaWidgetTools.qrc \
    res.qrc

DISTFILES += \
    README.md \
    Sapphire.rc \
    Sapphire.rep \
    SysIcons/imageres 023.png \
    SysIcons/imageres 050.png \
    SysIcons/imageres 051.png \
    SysIcons/imageres 105.png \
    style.json \
    uac.manifest












# INCLUDEPATH += $$PWD/include/Ela
# DEPENDPATH += $$PWD/include/Ela
