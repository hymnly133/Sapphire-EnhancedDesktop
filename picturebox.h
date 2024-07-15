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
    QPixmap source;
    QPixmap scaled;
    QBrush m_brush;
    QSize displaySize;
    QSize actualSize;

    int off_x,off_y;
    bool changed = true;
    double m_scale = 1.0;
    void updateDispaly();
protected:
    void paintEvent(QPaintEvent * event);

public slots:
    bool setImage(QPixmap &image);
    void setBackground(QBrush brush);
};
#endif
