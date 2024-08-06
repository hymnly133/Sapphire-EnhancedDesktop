#ifndef STYLESETTOTAL_H
#define STYLESETTOTAL_H

#include <QWidget>

namespace Ui {
class styleSetTotal;
}

class styleSetTotal : public QWidget
{
    Q_OBJECT

public:
    explicit styleSetTotal(QWidget *parent = nullptr);
    ~styleSetTotal();
    QString getFontName();

signals:
    void on_fontChangeBox_clicked();
    // void on_rebootBox_clicked();
    void on_resizeBox_clicked();

private slots:
    void on_ButtonLoadCustomIcons_pressed();

    void on_ButtonRecoverDefaultIcons_pressed();

private:
    Ui::styleSetTotal *ui;
};

#endif // STYLESETTOTAL_H
