#ifndef SMENU_H
#define SMENU_H

#include "SAnimationRect.h"
#include "saction.h"
#include <QWidget>
#include <qmenu.h>

struct SActionInfo {
    QString name;
    QString id;
    QString command;
    bool visible = true;
    QJsonObject to_json();
    void load_json(QJsonObject root);
    static SActionInfo sparetor();

};
class SMenu: public QMenu
{
    Q_OBJECT
public:


    //SAct::name统一使用显示名（即text）

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

    //sname(english):text(local);
    static QMap<QString, QString> group2name;

    // QList<QString> exitsCommands;
    QMap<QString, SAction*> sacts();

    QSize aimSize;
    QPoint previousPos;
    QPoint aimPos;
    QString path;

    bool ismain = false;
    bool firstShow = true;

    void init();
    void raiseAction(SAction* action);
    void raiseAction(QString actname);
    SAction *removeAction(QString actname);
    SAction* action(QString actname);

    void hideAction(QString actname);
    void hideAction(SAction* act);
    void showAction(QString actname);
    void showAction(SAction* act);

    void adjustSize();

    void exec(QPoint pos);

    //从已有列表中添加到菜单对象中的主要方法
    void addShellCommand(QPair<QString, SActionInfo> command);

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
    static void scanSysCommands();

    //Key==S时当作分隔符
    static QList<QPair<QString, SActionInfo>> dirCommands;
    static QList<QPair<QString, SActionInfo>> dirBGCommands;
    static QList<QPair<QString, SActionInfo>> desktopCommands;
    static QMap<QString, QList<QPair<QString, SActionInfo>>*> filesCommands;

    static QJsonArray list2json(const QList<QPair<QString, SActionInfo> > &list);
    static void json2list(QList<QPair<QString, SActionInfo> > &list, QJsonArray arr);


    // QJsonArray to_json();
    // void load_json(QJsonArray root);

    static void write_json();
    static void read_json(QJsonObject data);

    //load和scan的最终添加方法
    static void addToListFromInfo(QList<QPair<QString, SActionInfo>>& list, SActionInfo& info);


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
};

#endif // SMENU_H
