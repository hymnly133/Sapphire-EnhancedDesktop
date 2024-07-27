#ifndef ELAMENUBAR_H
#define ELAMENUBAR_H

#include <QMenuBar>

#include "Def.h"
#include "stdafx.h"

class ElaMenu;
class ELA_EXPORT ElaMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit ElaMenuBar(QWidget* parent = nullptr);
    ~ElaMenuBar();

    QAction* addMenu(QMenu* menu);
    ElaMenu* addMenu(const QString& title);
    ElaMenu* addMenu(const QIcon& icon, const QString& title);
    ElaMenu* addMenu(ElaIconType icon, const QString& title);

    QAction* addElaIconAction(ElaIconType icon, const QString& text);
    QAction* addElaIconAction(ElaIconType icon, const QString& text, const QKeySequence& shortcut);

protected:
    void resizeEvent(QResizeEvent*event) override;
};

#endif // ELAMENUBAR_H
