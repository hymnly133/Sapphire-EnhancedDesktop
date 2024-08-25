#include "picturebox.h"
#include "SysFunctions.h"
#include <QPainter>
#include <QDebug>
#include "guifunc.h"
#include "qmovie.h"
#include "style.h"


PictureBox::PictureBox(QWidget *parent, double m_scale) : QWidget(parent)
{
    source = nullptr;
    m_brush = QBrush(Qt::white);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    setMinimumSize(10, 10);
    setScale(m_scale);
    movieLabel = new QLabel(this);
}

void PictureBox::setBackground(QBrush brush)
{
    m_brush = brush;
    update();
}


void PictureBox::setScale(double scale)
{
    m_scale = scale;
    updateDispaly();
}

void PictureBox::setImage(QPixmap &image)
{
    if(image.isNull()) {
        return;
    }
    type = pic;

    // if(!followSource&& source!=nullptr)
    //     delete source;
    requireRefresh = true;
    source = new QPixmap(image);

    followSource = false;

    updateDispaly();
    return;
}

void PictureBox::follow(QPixmap *pPixmap)
{
    type = pic;
    requireRefresh = true;
    source = pPixmap;
    followSource = true;
    updateDispaly();
}

void PictureBox::setGIF(QString path)
{
    movie = new QMovie(path);
    movieLabel->setMovie(movie);
    type = gif;
    movie->start();
    updateDispaly();
}

void PictureBox::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    if(type == pic) {
        if(source != nullptr) {
            QPainter painter(this);
            // qDebug()<<scaled.size()<<size()<<off_x;


            painter.setBackground(m_brush);
            painter.drawPixmap(off_x, off_y, applyUntransparentRatio(scaled, untransparentRatio));
        }
    } else if(type == gif) {

    }
    paintSide(this, QColor("red"));
}

void PictureBox::updateDispaly()
{
    if((source != nullptr && !source->isNull()) || type == gif) {
        // qDebug()<<newSource<<aim_scale();
        if((aim_scale() == pre_scale) && !requireRefresh && (preParentSize == parentWidget()->size())) {
            return;
        }

        pre_scale = aim_scale();
        requireRefresh = false;
        preParentSize = parentWidget()->size();

        double window_width, window_height;
        double image_width, image_height;
        double r1, r2, r;

        window_width = parentWidget()->width();
        window_height = parentWidget()->height();


        if(type == pic) {
            image_width = source->width();
            image_height = source->height();
        } else {
            image_width = movie->currentPixmap().size().width();
            image_height = movie->currentPixmap().size().height();
        }

        r1 = window_width / image_width;
        r2 = window_height / image_height;

        if((enable_image_fill && maxFill)) {
            r = qMax(r1, r2);
        } else {
            r = qMin(r1, r2);
        }

        displaySize = QSize(image_width * r * aim_scale() + 1, image_height * r * aim_scale() + 1);

        actualSize = displaySize;
        off_x = 0;
        off_y = 0 ;


        if(displaySize.width() >= window_width) {
            actualSize.setWidth(window_width);
            off_x = -(displaySize.width() - window_width) / 2;
        }

        if(displaySize.height() >= window_height) {
            actualSize.setHeight(window_height);
            off_y = -(displaySize.height() - window_height) / 2;
        }


        if(type == gif) {
            movieLabel->move(off_x, off_y);
            movieLabel->setFixedSize(displaySize);
            movie->setScaledSize(displaySize);
        }


        scaled = source->scaled(displaySize
                                , Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        setFixedSize(actualSize);
    }
}

PictureBox::~PictureBox()
{

}
