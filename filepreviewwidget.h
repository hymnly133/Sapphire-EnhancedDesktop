#ifndef FILEPREVIEWWIDGET_H
#define FILEPREVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>  // 新增：用于支持滚动条

class FilePreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FilePreviewWidget(QWidget *parent = nullptr);

    void setText(const QString &text);
    void setImage(const QImage &image);

private:
    QTextEdit *contentTextEdit;  // 新增：用于支持滚动条
    QLabel *imageLabel;          // 新增：用于显示图片
    QVBoxLayout *layout;
};

#endif // FILEPREVIEWWIDGET_H
