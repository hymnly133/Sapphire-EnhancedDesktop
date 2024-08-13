#include "sglshower.h"
#include "qevent.h"

SGLShower::SGLShower(SLayout *layout): SUnit(layout)
{
    box = new GLPictureBox(this);
    box->move(0, 0);
    // box->setFixedSize(100,100);
    // box->adjustSize();
}

void SGLShower::showEvent(QShowEvent *event)
{
    // box->show();
    // box->show();
}

void SGLShower::double_click_action(QMouseEvent *event)
{
    box->show();
    // test();
}

void SGLShower::resizeEvent(QResizeEvent *event)
{
    box->resize(event->size());
    SUnit::resizeEvent(event);
}
