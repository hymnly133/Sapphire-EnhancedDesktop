#ifndef GLPICTUREBOX_H
#define GLPICTUREBOX_H

#include <QOpenGLWidget>
#include <QWidget>
#include<QOpenGLFunctions_4_5_Core>
#include<qopenglextrafunctions>

class GLPictureBox : public QOpenGLWidget,protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    GLPictureBox(QWidget* parent = nullptr);

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // GLPICTUREBOX_H
