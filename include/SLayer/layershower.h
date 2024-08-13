#ifndef LAYERSHOWER_H
#define LAYERSHOWER_H

#include "SAnimationRect.h"
#include <QWidget>

class MainWindow;
class LayerShower : public QWidget
{
    Q_OBJECT
public:
    explicit LayerShower(MainWindow *parent, int screenId);
    MainWindow* pmw;
    enum Layer {
        Bottom = 0,
        Upper = 2
    };
    int screenId;
    Layer layer;
    bool onBootAnimation = true;
    SAnimationRect* ar;
    QPoint finalPos;
    QSize finalSize;

    bool lowerShower = false;
    void clearTooltip();
    void clearInputDialog();
    void startBootAnimation();
signals:

protected:
    void paintEvent(QPaintEvent *event) override;




    // QWidget interface
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event) override;
public slots:
    void whenBootAnimationUpdation();
};

#endif // LAYERSHOWER_H
