#ifndef SETTINGABOUTPAGE_H
#define SETTINGABOUTPAGE_H

#include <QWidget>

namespace Ui {
class settingAboutPage;
}

class settingAboutPage : public QWidget
{
    Q_OBJECT

public:
    explicit settingAboutPage(QWidget *parent = nullptr);
    ~settingAboutPage();
    void setVersion(const QString& version);

private:
    Ui::settingAboutPage *ui;
};

#endif // SETTINGABOUTPAGE_H
