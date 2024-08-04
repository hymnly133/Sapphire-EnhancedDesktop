#include "sfieldswidget.h"



SFieldsWidget::SFieldsWidget(QWidget *parent):QWidget(parent)
{
    nowLayout = new QVBoxLayout();
    setLayout(nowLayout);

}

void SFieldsWidget::add(QStringList parentfields, QWidget *content, bool small)
{
    if(!fieldWidgets.contains(parentfields)){
        QWidget* fieldWidget = new QWidget(this);
        QHBoxLayout* fieldMainLayout = new QHBoxLayout();
        fieldWidget->setLayout(fieldMainLayout);
        QVBoxLayout* fieldSmallLayout = new QVBoxLayout();
        QVBoxLayout* fieldBigLayout = new QVBoxLayout();


        fieldMainLayout->addLayout(fieldSmallLayout,0);
        fieldMainLayout->addLayout(fieldBigLayout,2);



        fieldWidgets.insert(parentfields,fieldWidget);
        fieldContent.insert(parentfields,new QList<QWidget*>);

        bigLayouts.insert(parentfields,fieldBigLayout);
        smallLayouts.insert(parentfields,fieldSmallLayout);


        QLabel* nameLabel = new QLabel(parentWidget());
        nameLabel->setText(tr("来自：")+parentfields.join('/'));
        labels.insert(parentfields,nameLabel);


        nowLayout->addWidget(nameLabel);
        nowLayout->addWidget(fieldWidget);
    }

    content->setParent(fieldWidgets[parentfields]);
    if(small){
        smallLayouts[parentfields]->addWidget(content);
        ((QHBoxLayout*)fieldWidgets[parentfields]->layout())->setStretchFactor(smallLayouts[parentfields], 1);
    }
    else{
        bigLayouts[parentfields]->addWidget(content);
    }
    fieldContent[parentfields]->append(content);
}

void SFieldsWidget::raise(QStringList aimfield)
{
    if(!fieldWidgets.contains(aimfield)) return;
    //移出
    foreach (auto widget, fieldWidgets.values()) {
        widget->setParent(NULL);
    }
    foreach (auto widget, labels.values()) {
        widget->setParent(NULL);
    }

    delete this->layout();

    nowLayout = new QVBoxLayout(this);
    setLayout(nowLayout);

    foreach (auto widget, fieldWidgets.values()) {
        widget->setParent(this);
    }
    foreach (auto widget, labels.values()) {
        widget->setParent(this);
    }
    // nowLayout->addWidget(labels[aimfield]);
    labels[aimfield]->setVisible(false);
    nowLayout->addWidget(fieldWidgets[aimfield]);

    foreach (auto field, fieldWidgets.keys()) {
        if(field!=aimfield){
            nowLayout->addSpacing(100);
            labels[field]->setVisible(true);
            nowLayout->addWidget(labels[field]);
            nowLayout->addWidget(fieldWidgets[field]);
        }
    }
}
