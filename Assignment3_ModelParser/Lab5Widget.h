#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include "ObjParse.h"
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
  
  QMatrix4x4 model_;
  QMatrix4x4 view_;
  QMatrix4x4 projection_;

protected:
  // Required interaction overrides
  void keyReleaseEvent(QKeyEvent* keyEvent) override;

  // Required overrides form QOpenGLWidget
  void initializeGL() override;
  //void render() override;
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
 //void loadData(std::vector<float> verts, std::vector<float> colors, std::vector<int> idx);
void loadData(std::vector<float> verts2, std::vector<int> idx2);

 void releaseBuffers();
 virtual ~Lab5Widget();
  
  // Make sure we have some size that makes sense.
  QSize sizeHint() const {return QSize(800,600);}
  std::vector<float> bunnyVerts;
  std::vector<float> bunnyVertNormals;
  std::vector<std::vector<int>> bunnyFaces;
  std::vector<float> monkeyVerts;
  std::vector<float> monkeyVertNormals;
  std::vector<std::vector<int>> monkeyFaces;
};
