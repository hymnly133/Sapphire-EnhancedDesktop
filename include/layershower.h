#ifndef LAYERSHOWER_H
#define LAYERSHOWER_H

#include <QWidget>

class LayerShower : public QWidget
{
    Q_OBJECT
public:
    explicit LayerShower(QWidget *parent = nullptr);

    void Clear();
signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;



    // QWidget interface
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event) override;
};

#endif // LAYERSHOWER_H
