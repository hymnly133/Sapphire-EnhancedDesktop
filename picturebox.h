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
    bool requireFill =false;
    bool followSource = false;
    QPixmap* source;
    QPixmap scaled;
    QBrush m_brush;
    QSize displaySize;
    QSize actualSize;

    int off_x,off_y;
    double m_scale = 1.0;
    void updateDispaly();
protected:
    void paintEvent(QPaintEvent * event);

public slots:
    void setImage(QPixmap &image);
    void follow(QPixmap* pPixmap);
    void setBackground(QBrush brush);
};
#endif
