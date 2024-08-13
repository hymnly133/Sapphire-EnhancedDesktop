#include "glpicturebox.h"
#include "qdebug.h"
#include "qgl.h"
#include "style.h"
unsigned int VBO, VAO;
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;
float verticals[] = {
    -0.5, 0.5, 0,
    0.5, 0.5, 0,
    0, -0.5, -0.5
};

GLPictureBox::GLPictureBox(QWidget *parent): QOpenGLWidget(parent)
{
    // initializeGL();
}
const char *vertexShaderSource = "#version 450 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char*fragmentShaderSource = R"(#version 330 core
    out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";
void GLPictureBox::initializeGL()
{


    qDebug()<<initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
    const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
    const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
    //    const GLubyte* OpenGLExensions =glGetString(GL_EXTENSIONS); //

    QString str;
    str.sprintf("%s | %s | %s",name,biaoshifu,OpenGLVersion);

    qDebug()<<str;

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);


    glBufferData(GL_ARRAY_BUFFER,sizeof(verticals),verticals,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog ;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog ;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::Link_FAILED\n" << infoLog ;
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);




}

void GLPictureBox::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glFlush();
    update();
}

void GLPictureBox::paintGL()
{
    lower();
    // glClearColor(1.0f,1.0f,1.0f,0.001f);
    // glClear(GL_COLOR_BUFFER_BIT);
    // qDebug()<<"paint";
    // glUseProgram(shaderProgram);
    // glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES,0,3);
    QPainter painter(this);
    painter.setBrush(QBrush(highlight_color));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());
}
