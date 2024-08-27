#ifndef SMENU_H
#define SMENU_H

#include "SAnimationRect.h"
#include "saction.h"
#include <QWidget>
#include <qmenu.h>
struct SActionField {
    SActionField(QString name, QString id) {}
    QList<SActionInfo*> acts;
    QJsonObject to_json();
    QString name;
    QString id;
    void load_json(QJsonObject root);
    bool visible = true;
};


struct SActionInfo {
    QString name;
    QString id;
    QString command;
    SActionField* menu;
    bool visible = true;
    QJsonObject to_json();
    void load_json(QJsonObject root);
    static SActionInfo sparetor();
};
class MenuHelper: public QObject
{
public:
    enum CommandType {
        dir = 0,
        dirBG = 1,
        file = 2,
        desktop = 3,
    };
    enum Field {
        edit = 0,
        normal = 1
    };
    MenuHelper(QObject* parent = nullptr);;
    QMap<QString, QIcon> icons;
    QStringList ignoreCommands;

    QMap<QString, QString> group2name;
    void connectToAction();
    void connectToMenuu();
    void readDirReg(QString path);
    void readFileReg(QString group);
    void readFilesReg( );
    void readShellReg(QString path, CommandType type);
    void readAReg(QString path, CommandType type);

    //初始化系统命令的总方法
    void scanSysCommands();

    //一共有：
    //tray
    //new
    //system
    //more
    QList<SActionField*> menus;

    SActionField* tray;
    SActionField* setting;
    SActionField* create;
    SActionField* more;


    //Key==S时当作分隔符
    QList<SActionInfo> dirCommands;
    QList<SActionInfo> dirBGCommands;
    QList<SActionInfo> desktopCommands;
    QMap<QString, QList<SActionInfo>*> filesCommands;

    QJsonArray list2json(const QList<SActionInfo >& list);
    void json2list(QList<SActionInfo > & list, QJsonArray arr);

    void write_json();
    void read_json(QJsonObject data);

    //load和scan的最终添加方法
    void addToListFromInfo(QList<SActionInfo> &list, SActionInfo &info);
};


class SMenu: public QMenu
{
    Q_OBJECT
public:

    //SAct::name统一使用显示名（即text）
    SMenu(QWidget* parent = nullptr);
    SAnimationRect* arect;
    roundShower* rs;
    //标记是否有系统右键菜单
    bool added = false;
    SMenu* sysComMenu;
    SAction* syscomAct;
    bool alwaysRequireRefresh = false;


    void addAction(QAction* act);

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
    void addShellCommand(const SActionInfo &info);

    void addDirCommands();
    void addDirBGCommands();
    void addDesktopCommands();
    void addFileCommands(QString suffix);
    void addTrayCommands();
    void checkSysMenu();





    // QJsonArray to_json();
    // void load_json(QJsonArray root);






    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
};

#endif // SMENU_H
