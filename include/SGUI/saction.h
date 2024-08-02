#ifndef SACTION_H
#define SACTION_H

#include <QAction>


class SAction : public QAction
{
public:
    using QAction::QAction;
    bool multiCallable = false;
};

#endif // SACTION_H
