#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QWidget>
#include <QImage>
#include <QPixmap>


class PictureBox : public QWidget
{
    Q_OBJECT
public:
    explicit PictureBox(QWidget *parent=0,double m_scale =1.0);
    void setScale(double scale);
    ~PictureBox();
    bool maxFill = false;
    bool limitInisde = false;
    bool followSource = false;
    bool requireRefresh = true;
    QPixmap* source;
    QPixmap scaled;
    QBrush m_brush;
    QSize displaySize;
    QSize actualSize;
    QSize preParentSize = QSize();

    double aim_scale(){
        if(maxFill){
            return qMax(m_scale,1.0);
        }
        else{
            if(limitInisde){
                // qDebug()<<"Limited";
                 return qBound(0.01, m_scale, 0.95);
            }
            else
                return qBound(0.01, m_scale, 2.0);
        }
    };
    int off_x,off_y;
    double m_scale = 1.0;
    double pre_scale;
    void updateDispaly();
protected:
    void paintEvent(QPaintEvent * event);

public slots:
    void setImage(QPixmap &image);
    void follow(QPixmap* pPixmap);
    void setBackground(QBrush brush);
};
#endif
