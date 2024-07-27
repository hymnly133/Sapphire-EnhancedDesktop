#include "picturebox.h"
#include "SysFunctions.h"
#include <QPainter>
#include <QDebug>
#include "style.h"


PictureBox::PictureBox(QWidget *parent,double m_scale) : QWidget(parent)
{
    source = nullptr;
    m_brush = QBrush(Qt::white);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    setMinimumSize(10,10);
    setScale(m_scale);
}

void PictureBox::setBackground(QBrush brush)
{
    m_brush = brush;
    update();
}


void PictureBox::setScale(double scale){
    m_scale = scale;
    updateDispaly();
}

void PictureBox::setImage(QPixmap &image)
{
    if(image.isNull())
    {
        return;
    }

    // if(!followSource&& source!=nullptr)
    //     delete source;
    source = new QPixmap(image);

    followSource = false;

    updateDispaly();
    return;
}

void PictureBox::follow(QPixmap *pPixmap)
{
    // if(!followSource&& source!=nullptr)
    //     delete source;
    source = pPixmap;
    followSource = true;
    updateDispaly();
}

void PictureBox::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    if(source!=nullptr){
        QPainter painter(this);
        // qDebug()<<scaled.size()<<size()<<off_x;
        painter.setBackground(m_brush);
        painter.drawPixmap(off_x, off_y, scaled);
    }
    paintSide(this,QColor("red"));
}

void PictureBox::updateDispaly()
{
    if(source!=nullptr && !source->isNull()){
        double window_width, window_height;
        double image_width, image_height;
        double r1, r2, r;

        window_width = parentWidget()->width();
        window_height = parentWidget()->height();

        image_width = source->width();
        image_height = source->height();

        r1 = window_width / image_width;
        r2 = window_height / image_height;

        if((enable_image_fill&&maxFill))
            r = qMax(r1, r2);
        else
            r= qMin(r1,r2);

        displaySize =QSize(image_width * r * aim_scale()+1, image_height * r * aim_scale()+1);

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

        scaled = source->scaled(displaySize
                                , Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        setFixedSize(actualSize);
    }
}

PictureBox::~PictureBox()
{

}
