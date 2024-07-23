QT += core gui widgets winextras network concurrent quick qml

VERSION = 1.2.8.2
msvc{
    message("using msvc")
    QMAKE_CXXFLAGS += -execution-charset:utf-8
    QMAKE_CXXFLAGS += -source-charset:utf-8

    CONFIG(debug, debug|release) {
    message("debug");
        LIBS += -L$$PWD/lib/ -lelawidgettoolsd
    } else {
    message("release");
        LIBS += -L$$PWD/lib/ -lelawidgettools
    }
}

mingw{
message("using mingw")

LIBS += -L$$PWD/lib/ -lelawidgettools

}
QMAKE_LIBDIR += $$PWD/lib
INCLUDEPATH+= $$PWD/include
INCLUDEPATH+= $$PWD/include/ContextMenu
INCLUDEPATH+= $$PWD/include/Ela
INCLUDEPATH+= $$PWD/include/Ela/Example
DEPENDPATH += $$PWD/include/Ela

LIBS+= -luser32    # 使用WindowsAPI需要链接库
LIBS+= -ldwmapi
LIBS += -lOle32  -luuid -lshlwapi -luser32 -lshell32 -lsetupapi -lcomctl32



QMAKE_PROJECT_DEPTH = 0




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


SOURCES += SysFunctions.cpp \
    filefunc.cpp \
    include/picturebox.cpp \
    include/repaintcounterunit.cpp \
    include/roundshower.cpp \
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
    include/Ela/Example/T_BaseComponents.cpp \
    include/Ela/Example/T_ElaScreen.cpp \
    include/Ela/Example/T_Home.cpp \
    include/Ela/Example/T_Icon.cpp \
    include/Ela/Example/T_IconDelegate.cpp \
    include/Ela/Example/T_IconModel.cpp \
    include/Ela/Example/T_LogModel.cpp \
    include/Ela/Example/T_LogWidget.cpp \
    include/Ela/Example/T_Popup.cpp \
    include/Ela/Example/T_TabWidget.cpp \
    include/Ela/Example/T_UpdateWidget.cpp \
    include/settingwindow.cpp\

HEADERS += \
    include/ContextMenu/contextmenu.h \
    include/ContextMenu/desktopmenu.h \
    include/ContextMenu/shellitem.h \
    include/ContextMenu/shellmemmanager.h \
    include/ContextMenu/shellmenuitem.h \
    include/Ela/Def.h \
    include/Ela/ElaAcrylicUrlCard.h \
    include/Ela/ElaAppBar.h \
    include/Ela/ElaApplication.h \
    include/Ela/ElaBreadcrumbBar.h \
    include/Ela/ElaCheckBox.h \
    include/Ela/ElaComboBox.h \
    include/Ela/ElaContentDialog.h \
    include/Ela/ElaDockWidget.h \
    include/Ela/ElaDoubleSpinBox.h \
    include/Ela/ElaDxgiManager.h \
    include/Ela/ElaEventBus.h \
    include/Ela/ElaExponentialBlur.h \
    include/Ela/ElaFlowLayout.h \
    include/Ela/ElaGraphicsItem.h \
    include/Ela/ElaGraphicsLineItem.h \
    include/Ela/ElaGraphicsScene.h \
    include/Ela/ElaGraphicsView.h \
    include/Ela/ElaIcon.h \
    include/Ela/ElaIconButton.h \
    include/Ela/ElaImageCard.h \
    include/Ela/ElaInteractiveCard.h \
    include/Ela/ElaLineEdit.h \
    include/Ela/ElaListView.h \
    include/Ela/ElaLog.h \
    include/Ela/ElaMenu.h \
    include/Ela/ElaMenuBar.h \
    include/Ela/ElaMessageBar.h \
    include/Ela/ElaMessageButton.h \
    include/Ela/ElaMultiSelectComboBox.h \
    include/Ela/ElaNavigationBar.h \
    include/Ela/ElaNavigationRouter.h \
    include/Ela/ElaProgressBar.h \
    include/Ela/ElaPushButton.h \
    include/Ela/ElaRadioButton.h \
    include/Ela/ElaReminderCard.h \
    include/Ela/ElaScrollArea.h \
    include/Ela/ElaScrollBar.h \
    include/Ela/ElaScrollPage.h \
    include/Ela/ElaScrollPageArea.h \
    include/Ela/ElaSlider.h \
    include/Ela/ElaSpinBox.h \
    include/Ela/ElaStatusBar.h \
    include/Ela/ElaSuggestBox.h \
    include/Ela/ElaTabBar.h \
    include/Ela/ElaTabWidget.h \
    include/Ela/ElaTableView.h \
    include/Ela/ElaText.h \
    include/Ela/ElaToggleButton.h \
    include/Ela/ElaToggleSwitch.h \
    include/Ela/ElaToolBar.h \
    include/Ela/ElaToolButton.h \
    include/Ela/ElaWidget.h \
    include/Ela/ElaWindow.h \
    include/Ela/Example/T_BaseComponents.h \
    include/Ela/Example/T_ElaScreen.h \
    include/Ela/Example/T_Home.h \
    include/Ela/Example/T_Icon.h \
    include/Ela/Example/T_IconDelegate.h \
    include/Ela/Example/T_IconModel.h \
    include/Ela/Example/T_LogModel.h \
    include/Ela/Example/T_LogWidget.h \
    include/Ela/Example/T_Popup.h \
    include/Ela/Example/T_TabWidget.h \
    include/Ela/Example/T_UpdateWidget.h \
    include/Ela/singleton.h \
    include/Ela/stdafx.h \
    include/SysFunctions.h \
    include/filefunc.h \
    include/layershower.h \
    include/mainwindow.h \
    include/picturebox.h \
    include/repaintcounterunit.h \
    include/roundshower.h \
    include/sbgshower.h \
    include/sblockcontainer.h \
    include/sblocklayout.h \
    include/scontainer.h \
    include/screenfunc.h \
    include/sdock.h \
    include/seditbox.h \
    include/settingwindow.h \
    include/sfile.h \
    include/shidetextblock.h \
    include/slayout.h \
    include/slinearlayout.h \
    include/smultifunc.h \
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
    include/Ela/Font/ElaAwesome.ttf \
    include/Ela/Image/Cirno.jpg \
    include/Ela/Image/Home_Background.png \
    include/Ela/Image/Moon.jpg \
    include/Ela/Image/control/Acrylic.png \
    include/Ela/Image/control/AnimatedIcon.png \
    include/Ela/Image/control/AnimatedVisualPlayer.png \
    include/Ela/Image/control/AnimationInterop.png \
    include/Ela/Image/control/AppBarButton.png \
    include/Ela/Image/control/AppBarSeparator.png \
    include/Ela/Image/control/AppBarToggleButton.png \
    include/Ela/Image/control/AutoSuggestBox.png \
    include/Ela/Image/control/AutomationProperties.png \
    include/Ela/Image/control/Border.png \
    include/Ela/Image/control/BreadcrumbBar.png \
    include/Ela/Image/control/Button.png \
    include/Ela/Image/control/CalendarDatePicker.png \
    include/Ela/Image/control/CalendarView.png \
    include/Ela/Image/control/Canvas.png \
    include/Ela/Image/control/Checkbox.png \
    include/Ela/Image/control/Clipboard.png \
    include/Ela/Image/control/ColorPaletteResources.png \
    include/Ela/Image/control/ColorPicker.png \
    include/Ela/Image/control/ComboBox.png \
    include/Ela/Image/control/CommandBar.png \
    include/Ela/Image/control/CommandBarFlyout.png \
    include/Ela/Image/control/CompactSizing.png \
    include/Ela/Image/control/ConnectedAnimation.png \
    include/Ela/Image/control/ContentDialog.png \
    include/Ela/Image/control/CreateMultipleWindows.png \
    include/Ela/Image/control/DataGrid.png \
    include/Ela/Image/control/DatePicker.png \
    include/Ela/Image/control/DropDownButton.png \
    include/Ela/Image/control/EasingFunction.png \
    include/Ela/Image/control/Expander.png \
    include/Ela/Image/control/FilePicker.png \
    include/Ela/Image/control/FlipView.png \
    include/Ela/Image/control/Flyout.png \
    include/Ela/Image/control/Grid.png \
    include/Ela/Image/control/GridView.png \
    include/Ela/Image/control/HyperlinkButton.png \
    include/Ela/Image/control/IconElement.png \
    include/Ela/Image/control/Image.png \
    include/Ela/Image/control/ImplicitTransition.png \
    include/Ela/Image/control/InfoBadge.png \
    include/Ela/Image/control/InfoBar.png \
    include/Ela/Image/control/InkCanvas.png \
    include/Ela/Image/control/InkToolbar.png \
    include/Ela/Image/control/InputValidation.png \
    include/Ela/Image/control/ItemsRepeater.png \
    include/Ela/Image/control/Line.png \
    include/Ela/Image/control/ListBox.png \
    include/Ela/Image/control/ListView.png \
    include/Ela/Image/control/MediaPlayerElement.png \
    include/Ela/Image/control/MenuBar.png \
    include/Ela/Image/control/MenuFlyout.png \
    include/Ela/Image/control/NavigationView.png \
    include/Ela/Image/control/NumberBox.png \
    include/Ela/Image/control/PageTransition.png \
    include/Ela/Image/control/ParallaxView.png \
    include/Ela/Image/control/PasswordBox.png \
    include/Ela/Image/control/PersonPicture.png \
    include/Ela/Image/control/PipsPager.png \
    include/Ela/Image/control/Pivot.png \
    include/Ela/Image/control/ProgressBar.png \
    include/Ela/Image/control/ProgressRing.png \
    include/Ela/Image/control/PullToRefresh.png \
    include/Ela/Image/control/RadialGradientBrush.png \
    include/Ela/Image/control/RadioButton.png \
    include/Ela/Image/control/RadioButtons.png \
    include/Ela/Image/control/RatingControl.png \
    include/Ela/Image/control/RelativePanel.png \
    include/Ela/Image/control/RepeatButton.png \
    include/Ela/Image/control/RevealFocus.png \
    include/Ela/Image/control/RichEditBox.png \
    include/Ela/Image/control/RichTextBlock.png \
    include/Ela/Image/control/ScrollViewer.png \
    include/Ela/Image/control/SemanticZoom.png \
    include/Ela/Image/control/Shape.png \
    include/Ela/Image/control/Slider.png \
    include/Ela/Image/control/Sound.png \
    include/Ela/Image/control/SplitButton.png \
    include/Ela/Image/control/SplitView.png \
    include/Ela/Image/control/StackPanel.png \
    include/Ela/Image/control/StandardUICommand.png \
    include/Ela/Image/control/SwipeControl.png \
    include/Ela/Image/control/TabView.png \
    include/Ela/Image/control/TeachingTip.png \
    include/Ela/Image/control/TextBlock.png \
    include/Ela/Image/control/TextBox.png \
    include/Ela/Image/control/ThemeTransition.png \
    include/Ela/Image/control/TimePicker.png \
    include/Ela/Image/control/TitleBar.png \
    include/Ela/Image/control/ToggleButton.png \
    include/Ela/Image/control/ToggleSplitButton.png \
    include/Ela/Image/control/ToggleSwitch.png \
    include/Ela/Image/control/ToolTip.png \
    include/Ela/Image/control/TreeView.png \
    include/Ela/Image/control/VariableSizedWrapGrid.png \
    include/Ela/Image/control/Viewbox.png \
    include/Ela/Image/control/WebView.png \
    include/Ela/Image/control/XamlUICommand.png \
    include/Ela/Image/github.png \
    include/Ela/Image/noise.png \
    style.json












INCLUDEPATH += $$PWD/include/Ela
DEPENDPATH += $$PWD/include/Ela
