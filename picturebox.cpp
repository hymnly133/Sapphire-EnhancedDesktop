#include "picturebox.h"
#include "SysFunctions.h"
#include <QPainter>
#include <QDebug>
#include "style.h"
static const int IMAGE_WIDTH = 300;
static const int IMAGE_HEIGHT = 300;
static const QSize IMAGE_SIZE = QSize(IMAGE_WIDTH, IMAGE_HEIGHT);


PictureBox::PictureBox(QWidget *parent,double m_scale) : QWidget(parent)
{
    source = QPixmap(IMAGE_SIZE);
    source.fill();
    m_brush = QBrush(Qt::white);
    setScale(m_scale);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void PictureBox::setBackground(QBrush brush)
{
    m_brush = brush;
    update();
}


void PictureBox::setScale(double scale){

    m_scale = qBound(0.01, scale, 2.0);
    updateDispaly();
}

bool PictureBox::setImage(QPixmap &image)
{
    if(image.isNull())
    {
        return false;
    }
    source = image;
    updateDispaly();
    return true;
}

void PictureBox::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBackground(m_brush);
    // painter.eraseRect(rect());
    painter.drawPixmap(off_x, off_y, scaled);
    paintSide(this,QColor("red"));
}

void PictureBox::updateDispaly()
{

    double window_width, window_height;
    double image_width, image_height;
    double r1, r2, r;

    window_width = parentWidget()->width();
    window_height = parentWidget()->height();

    image_width = source.width();
    image_height = source.height();

    r1 = window_width / image_width;
    r2 = window_height / image_height;

    if(enable_image_fill)
        r = qMax(r1, r2);
    else
        r= qMin(r1,r2);

    displaySize =QSize(image_width * r * m_scale+1, image_height * r * m_scale+1);

    actualSize = displaySize;
    off_x = 0;
    off_y = 0 ;


    if(displaySize.width()>=window_width){
        actualSize.setWidth(window_width);
        off_x = -(displaySize.width()-window_width)/2;
    }

    if(displaySize.height()>=window_height){
        actualSize.setHeight(window_height);
        off_y = -(displaySize.height()-window_height)/2;
    }


    changed = true;

    scaled = source.scaled(displaySize
                           , Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

\


    setFixedSize(actualSize);
}

PictureBox::~PictureBox()
{

}
