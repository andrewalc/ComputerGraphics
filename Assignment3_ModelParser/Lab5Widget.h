#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>

// TODO:  This macro should work when both true AND false
#define USE_QT_OPENGL true
// ENDTODO

/**
 * This is just a basic OpenGL widget that will allow a change of background color.
 */
class Lab5Widget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

private:
  QString vertexShaderString() const;
  QString fragmentShaderString() const;
  void createShader();
  QOpenGLVertexArrayObject vao_;

protected:
  // Required interaction overrides
  void keyReleaseEvent(QKeyEvent* keyEvent) override;

  // Required overrides form QOpenGLWidget
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

#if USE_QT_OPENGL
  QOpenGLBuffer vbo_;
  QOpenGLBuffer ibo_;
  QOpenGLBuffer cbo_;
  QOpenGLShaderProgram shaderProgram_;
#else
  GLuint vboID_;
  GLuint iboID_;
  GLuint cboID_;
  unsigned int shaderID_;

  unsigned int compileShader(unsigned int type, const QString& shaderSrc);
  
#endif

  
  
public:
 Lab5Widget(QWidget* parent=nullptr);
 void loadData(const GLfloat* verts, const GLfloat* colors, const GLuint* idx);
 void releaseBuffers();
 virtual ~Lab5Widget();
  
  // Make sure we have some size that makes sense.
  QSize sizeHint() const {return QSize(800,600);}
};
