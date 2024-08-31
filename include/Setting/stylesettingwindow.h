#ifndef STYLESETTINGWINDOW_H
#define STYLESETTINGWINDOW_H
#include "qdialog.h"
#include "qobject.h"
#include "qtreewidget.h"
#include "sfieldswidget.h"
#include "stylesettotal.h"
#include "ui_styleSetting.h"

class StyleSettingWindow: public QDialog
{
    Q_OBJECT
public:
    StyleSettingWindow();


    Ui::Form* ui;
    //将整个content按照fields类型设置到面板
    void setInLayout(QStringList fields, QWidget* content, bool issmall);
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void on_fontChangeBox_clicked();
    void on_rebootBox_clicked();
    void on_resizeBox_clicked();

    // void onListClicked(QListWidgetItem *item);
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:

    // 初始化各个主空间，Color等
    void iniFieldWidget(const QStringList& fields);

    //《关于》页面的制作
    void IniAboutPage();

    // 添加值参数参数的控件
    void processVals();

    // 添加布尔值控件
    void processBoolValues();
    // 添加整数值控件
    void processIntValues();
    // 添加浮点数值控件
    void processDoubleValues();
    // 添加颜色控件
    void processColorValues();

    //连接treeView与stackedWidget
    void setTreeView(const QStringList &fields);

    //根据fields添加TreeItem;
    void addAItem(const QStringList &fields);

    //添加内容的主方法
    void addContent(const QStringList &fields, QWidget* widget, bool issmall = false);



private:
    //field与fieldWidget
    QMap<QString, SFieldsWidget*> field2widget;
    QMap<QString, int> field2ind;
    QMap<QStringList, SFieldsWidget*> fields2widget;
    QMap<QStringList, QTreeWidgetItem*> fields2treeItem;
    QMap<QTreeWidgetItem*, int> item2ind;
    QMap<QTreeWidgetItem*, QStringList> item2fields;
    QMap<int, SFieldsWidget*> ind2widget;
    //fields与widgetz
    // QMap<QString, QWidget*> m_widgets;
    styleSetTotal* m_totalWidget;
};


#endif // STYLESETTINGWINDOW_H
