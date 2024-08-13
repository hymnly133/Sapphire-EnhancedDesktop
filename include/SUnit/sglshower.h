#ifndef SGLSHOWER_H
#define SGLSHOWER_H

#include <QObject>
#include "glpicturebox.h"
#include "sunit.h"

class SGLShower : public SUnit
{
    Q_OBJECT
public:
    SGLShower(SLayout* layout = nullptr);
    GLPictureBox* box;
    void showEvent(QShowEvent* event);

    // SUnit interface
public:
    void double_click_action(QMouseEvent *event) override;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};


Q_DECLARE_METATYPE(SGLShower);
#endif // SGLSHOWER_H
