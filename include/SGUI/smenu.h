#ifndef SMENU_H
#define SMENU_H

#include "SAnimationRect.h"
#include "saction.h"
#include <QWidget>
#include <qmenu.h>

class SMenu: public QMenu
{
    Q_OBJECT
public:
    enum CommandType {
        dir = 0,
        dirBG = 1,
        file = 2,
        desktop = 3
    };
    SMenu(QWidget* parent = nullptr);
    SAnimationRect* arect;
    roundShower* rs;
    //标记是否有系统右键菜单
    bool added = false;
    SMenu* sysComMenu;
    SAction* syscomAct;
    static QMap<QString, QIcon> icons;
    static QStringList ignoreCommands;

    QList<QString> exitsCommands;
    QSize aimSize;
    QPoint previousPos;
    QPoint aimPos;
    QString path;

    bool ismain = false;
    bool firstShow = true;

    void init();
    void raiseAction(QAction* action);
    void exec(QPoint pos, bool multi = false);

    void addShellCommand(QPair<QString, QString> command);

    void addDirCommands();
    void addDirBGCommands();
    void addDesktopCommands();
    void addFileCommands(QString suffix);
    void addTrayCommands();
    void checkSysMenu();

    static void readDirReg(QString path);
    static void readFileReg(QString group);
    static void readFilesReg( );
    static void readShellReg(QString path, CommandType type);
    static void readAReg(QString path, CommandType type);

    //初始化系统命令的总方法
    static void initSysCommands();

    //Key==S时当作分隔符
    static QList<QPair<QString, QString>> dirCommands;
    static QList<QPair<QString, QString>> dirBGCommands;
    static QList<QPair<QString, QString>> DesktopCommands;
    static QMap<QString, QList<QPair<QString, QString>>*> filesCommands;


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
};

#endif // SMENU_H
