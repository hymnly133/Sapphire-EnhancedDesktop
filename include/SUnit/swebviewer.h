#ifndef SWEBVIEWER_H
#define SWEBVIEWER_H

#include <QWidget>
#include "sunit.h"

class SWebViewer : public SUnit
{
    Q_OBJECT
public:
    SWebViewer(SLayout* dis = nullptr, int sizex = 1, int sizey = 1);
};
Q_DECLARE_METATYPE(SWebViewer);
#endif // SWEBVIEWER_H
