#include "filepreviewwidget.h"
#include <QTextEdit>
#include <QPixmap>

FilePreviewWidget::FilePreviewWidget(QWidget *parent)
    : QWidget(parent)
{
    contentTextEdit = new QTextEdit(this);  // 新增：用于支持滚动条
    contentTextEdit->setReadOnly(true);  // 设置为只读
    contentTextEdit->setAlignment(Qt::AlignCenter);

    imageLabel = new QLabel(this);  // 新增：用于显示图片
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setVisible(false);  // 默认隐藏

    layout = new QVBoxLayout(this);
    layout->addWidget(contentTextEdit);
    layout->addWidget(imageLabel);  // 新增：将图片标签添加到布局中
    setLayout(layout);

    setWindowFlags(Qt::ToolTip);
}

void FilePreviewWidget::setText(const QString &text)
{
    contentTextEdit->setPlainText(text);  // 使用 QTextEdit 显示文本
    contentTextEdit->setVisible(true);    // 显示文本框
    imageLabel->setVisible(false);        // 隐藏图片标签
}

void FilePreviewWidget::setImage(const QImage &image)
{
    imageLabel->setPixmap(QPixmap::fromImage(image).scaled(200, 200, Qt::KeepAspectRatio));  // 显示图片
    imageLabel->setVisible(true);         // 显示图片标签
    contentTextEdit->setVisible(false);   // 隐藏文本框
}
