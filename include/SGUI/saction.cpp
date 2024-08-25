#include "saction.h"
#include "SysFunctions.h"
#include "qjsonobject.h"
#include "smenu.h"


QString SAction::path()
{
    return smenu->path;
}

void SAction::runInfo()
{
    shellContextMenuRun(strcommand, path());
}

void SAction::bindInfo(SActionInfo* info)
{
    // this->info = info;
    strcommand = info->command;
    setName(info->name);
    setVisible(info->visible);
    connect(this, &QAction::triggered, this, [ = ]() {
        runInfo();
    });
}

void SAction::setName(QString name)
{
    this->name = name;
    setText(name);
}

// QJsonObject SAction::to_json()
// {
//     QJsonObject root;
//     root.insert("name", name);
//     root.insert("command", stringCommand);
//     root.insert("visible", isVisible());
//     return root;
// }

// void SAction::load_json(QJsonObject root)
// {
//     setName(root.value("name").toString());
//     stringCommand = root.value("command").toString();
//     setVisible(root.value("name").toBool());
// }

SAction *SAction::fromInfo(SActionInfo *info)
{
    SAction* act = new SAction();
    act->bindInfo(info);
    return act;
}
