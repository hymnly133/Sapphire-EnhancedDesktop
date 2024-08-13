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
    enum State {
        bootIn,
        bootOn,
        bootOut,
        normal
    };

    int screenId;
    Layer layer;
    State state = bootIn;
    SAnimationRect* ar;
    QPoint finalPos;
    QSize finalSize;

    void clearTooltip();
    void clearInputDialog();
signals:
    void bootAnimationInStart();
    void bootAnimationInEnd();
    void bootAnimationOutStart();
    void bootAnimationOutEnd();
protected:
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event) override;
public slots:
    void startBootAnimationIn();
    void startBootAnimationOut();
    void whenBootAnimationUpdation();
};

#endif // LAYERSHOWER_H
