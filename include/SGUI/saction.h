#ifndef SACTION_H
#define SACTION_H


#include <QAction>

class SMenu;
struct SActionInfo;
class SAction : public QAction
{
public:
    using QAction::QAction;
    QString name;
    QString path();
    SMenu* smenu;
    QString strcommand;

    // bool multiCallable = false;
    void runInfo();
    void bindInfo(SActionInfo* info);
    void setName(QString name);

    // QJsonObject to_json();
    // void load_json(QJsonObject root);
    static SAction* fromInfo(SActionInfo* info);
};
#endif // SACTION_H
