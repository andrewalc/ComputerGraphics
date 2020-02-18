#include "Lab5Widget.h"

//////////////////////////////////////////////////////////////////////
// Publics

  // TODO:  Add vertex and index data to draw two triangles
  // Define our verts
  static const GLfloat verts[12] =
  {
	-0.8f, -0.8f, 0.0f, // Left vertex position
	0.8f, -0.8f, 0.0f,  // right vertex position
	-0.8f,  0.8f, 0.0f,  // Top vertex position
    0.8f, 0.8f, 0.0f
  };
  // Define our vert colors
  static const GLfloat colors[16] =
  {
      1.0f, 0.0f, 0.0f, 1.0f, // red
      0.0f, 1.0f, 0.0f, 1.0f, // green
      0.0f, 0.0f, 1.0f, 1.0f, // blue
      1.0f, 1.0f, 0.0f, 1.0f  // yellow
  };
  // Define our vert colors
  static const GLfloat newcolors[16] =
  {
      0.5f, 0.5f, 0.5f, 1.0f, // red
      0.0f, 1.0f, 0.0f, 1.0f, // green
      0.0f, 0.0f, 1.0f, 1.0f, // blue
      1.0f, 1.0f, 0.0f, 1.0f  // yellow
  };
  // Define our indices
  static const GLuint idx[6] =
  {
      0, 1, 2, 2, 1, 3
  };
bool TRIANGLE = true;
#if USE_QT_OPENGL
  Lab5Widget::Lab5Widget(QWidget* parent) : QOpenGLWidget(parent), vbo_(QOpenGLBuffer::VertexBuffer), cbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer)
#else
  Lab5Widget::Lab5Widget(QWidget* parent) : QOpenGLWidget(parent)
#endif
{
  setFocusPolicy(Qt::StrongFocus);
  // Load Cube
  ObjParse cubeParse = ObjParse();
  cubeParse.parse("objects/cube.obj");
  cubeVerts = cubeParse.verts;
  cubeVertNormals = cubeParse.vertNormals;
  cubeFaces = cubeParse.faces;
  // for (int i = 0; i < cubeParse.faces.size(); i++){
  //   if (cubeParse.faces[i].size() > 0) {
  //     GLfloat face[cubeParse.faces[i].size()];
  //     for (int j = 0; j < cubeParse.faces[i].size() > 0) {
  //     }
  //   }
  // }
  // for (int i = 0; i < cubeParse.verts.size(); i++) {
  //   std::cout << cubeVerts[i] << "\n";
  // }
}

void Lab5Widget::releaseBuffers() {
  vbo_.release();
  ibo_.release();
  cbo_.release();
  vao_.release();
}

Lab5Widget::~Lab5Widget()
{
#if USE_QT_OPENGL
  releaseBuffers();
  vbo_.destroy();
  ibo_.destroy();
  cbo_.destroy();
  vao_.destroy();
#else
  glDeleteProgram(shaderID_);
  glDeleteBuffers(1, &vboID_);
  glDeleteBuffers(1, &cboID_);
  glDeleteBuffers(1, &iboID_);
#endif
}

//////////////////////////////////////////////////////////////////////
// Privates
QString Lab5Widget::vertexShaderString() const
{
  QString str =
	"#version 330\n"
	"layout(location = 0) in vec3 position;\n"
    "layout(location = 1) in vec4 color;\n"
    "out vec4 vertColor;\n"
	"void main()\n"
	"{\n"
	"  gl_Position = vec4(position, 1.0);\n"
    "  vertColor = color;\n"
    "}\n";
  return str;
}

QString Lab5Widget::fragmentShaderString() const
{
  QString str =
	"#version 330\n"
    "in vec4 vertColor;\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"  color = vertColor;\n"
	"}\n";
  return str;
}

#if USE_QT_OPENGL
void Lab5Widget::createShader()
{
  QOpenGLShader vert(QOpenGLShader::Vertex);
  vert.compileSourceCode(vertexShaderString());
  QOpenGLShader frag(QOpenGLShader::Fragment);
  frag.compileSourceCode(fragmentShaderString());
  bool ok = shaderProgram_.addShader(&vert);
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
  ok = shaderProgram_.addShader(&frag);
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
  ok = shaderProgram_.link();
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
}
#else
void Lab5Widget::createShader()
{
  unsigned int prog = glCreateProgram();
  unsigned int vert = compileShader(GL_VERTEX_SHADER, vertexShaderString());
  unsigned int frag = compileShader(GL_FRAGMENT_SHADER, fragmentShaderString());
  glAttachShader(prog, vert);
  glAttachShader(prog, frag);
  glLinkProgram(prog);
  glValidateProgram(prog);

  // The shaders are build and ready to use.  We can now detach the individual shaders and destroy them
  glDetachShader(prog, vert);
  glDetachShader(prog, frag);
  glDeleteShader(vert);
  glDeleteShader(frag);

  // Assign here in case there are failures we need to bail on - This has not yet been implemented, though.
  shaderID_ = prog;
}

unsigned int Lab5Widget::compileShader(unsigned int type, const QString& shaderSrc)
{
  unsigned int id = glCreateShader(type);
  // Make sure we can create the shader
  if(id == 0) {
	return 0;
  }
  QByteArray ba = shaderSrc.toLocal8Bit();
  const char* src = ba.data();
  // Set the source
  glShaderSource(id, 1, &src, nullptr);
  // Compile
  glCompileShader(id);
  // Retrieve the result of our compilation
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE){
	int length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
	char* errorMessages = new char[length]; // Could also use alloca here.
	glGetShaderInfoLog(id, length, &length, errorMessages);
	if(type == GL_VERTEX_SHADER){
	  qDebug() << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessages << "\n";
	}else if(type == GL_FRAGMENT_SHADER){
	  qDebug() << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << errorMessages << "\n";
	}
	// Reclaim our memory
	delete[] errorMessages;
	// Delete our broken shader
	glDeleteShader(id);
	return 0;
  }

  return id;
}
#endif
///////////////////////////////////////////////////////////////////////
// Protected
void Lab5Widget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  // TODO
  // Handle key events here.
  if (keyEvent->key() == Qt::Key_Left) {
    // TRIANGLE = true;
    releaseBuffers();
    // loadData(verts, colors, idx);
    qDebug() << "Left Arrow Pressed";
    update();  // We call update after we handle a key press to trigger a redraw when we are ready
  } else if (keyEvent->key() == Qt::Key_Right) {
    // TRIANGLE = false;
    releaseBuffers();
    std::vector<int> cubeIndexes; 
    for (int i = 0; i < cubeFaces.size(); i++){
      if (cubeFaces[i].size() > 0) {
        for (int j = 0; j < cubeFaces[i].size(); j++) {
          if (j % 2 == 0) {
            std::cout << cubeFaces[i][j] << "\n";
            cubeIndexes.push_back(cubeFaces[i][j]);
          }
        }
      }
    }
    loadData(cubeVerts, cubeIndexes);
    qDebug() << "Right Arrow Pressed";
    update();  // We call update after we handle a key press to trigger a redraw when we are ready
  } else {
    qDebug() << "You Pressed an unsupported Key!";
  }
  // ENDTODO
}

//void Lab5Widget::loadData(std::vector<float> verts, std::vector<float> colors, std::vector<int> idx) {
void Lab5Widget::loadData(std::vector<float> verts, std::vector<int> idx) {
  releaseBuffers();
  
  GLfloat convertVerts[verts.size()];
  std::copy(verts.begin(), verts.end(), convertVerts);

  // normals
  // GLfloat convertVertNormals[vertNormals.size()];
  // std::copy(vertNormals.begin(), vertNormals.end(), convertVertNormals);

  GLfloat convertIndex[idx.size()];
  std::copy(idx.begin(), idx.end(), convertIndex);

  // Temporary bind of our shader.
  shaderProgram_.bind();

  // Bind our vbo inside our vao
  vbo_.bind();
  vbo_.allocate(convertVerts, verts.size() * sizeof(GL_FLOAT));


  // //Bind our vbo inside our vao
  // cbo_.bind();
  // cbo_.allocate(colors, 16 * sizeof(GL_FLOAT));  
  // //ENDTODO

  // Bind our vbo inside our vao
  ibo_.bind();
  ibo_.allocate(convertIndex, idx.size() * sizeof(GL_FLOAT));  
  // ENDTODO

  vao_.bind();
  vbo_.bind();
  shaderProgram_.enableAttributeArray(0);
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3);
  // cbo_.bind();
  // shaderProgram_.enableAttributeArray(1);
  // shaderProgram_.setAttributeBuffer(1, GL_FLOAT, 0, 4);
  ibo_.bind();
  // Releae the vao THEN the vbo
  vao_.release();
  shaderProgram_.release();
}
void Lab5Widget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();

  QOpenGLContext* curContext = this->context();
  qDebug() << "[Lab5Widget]::initializeGL() -- Context Information:";
  qDebug() << "  Context Valid: " << std::string(curContext->isValid() ? "true" : "false").c_str();
  qDebug() << "  GL Version Used: " << curContext->format().majorVersion() << "." << curContext->format().minorVersion();
  qDebug() << "  Vendor: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  qDebug() << "  Renderer: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
  qDebug() << "  Version: " << reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << "  GLSL Version: " << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

  // Set up our shaders.
  createShader();
  // ENDTODO
  // Set up our buffers and our vao
#if USE_QT_OPENGL
// Create and prepare a vbo
vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
vbo_.create();
// TODO:  Generate our color buffer
cbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
cbo_.create();
// TODO:  Generate our index buffer
ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
ibo_.create();
// Create a VAO to keep track of things for us.
vao_.create();
// loadData(verts, colors, idx);
#else
  vao_.create();
  vao_.bind();

  glGenBuffers(1, &vboID_);
  glBindBuffer(GL_ARRAY_BUFFER, vboID_);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GL_FLOAT), verts, GL_STATIC_DRAW);

  // TODO:  Generate our color buffer
  glGenBuffers(1, &cbo_ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cbo_ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 16 * sizeof(GL_FLOAT), colors, GL_STATIC_DRAW);
  // ENDTODO
  // TODO:  Generate our index buffer
  glGenBuffers(1, &ibo_ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GL_FLOAT), idx, GL_STATIC_DRAW);
  // ENDTODO

  vao_.release();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
  glViewport(0, 0, width(), height());
}

void Lab5Widget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
}

void Lab5Widget::paintGL()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(1.f, 1.f, 1.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if USE_QT_OPENGL
  shaderProgram_.bind();
  vao_.bind();
  // TODO: Change number of indices drawn
  glDrawElements(GL_TRIANGLES, TRIANGLE ? 3 : 6, GL_UNSIGNED_INT, 0);
  // ENDTODO
  vao_.release();
  shaderProgram_.release();
#else
  glUseProgram(shaderID_);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vboID_);
  glVertexAttribPointer(0,        // Attribute 0 matches our layout for vertex positions
      3,        // Size
      GL_FLOAT, // Type
      GL_FALSE, // Not normalized
      0,        // Stride - no interleaving
      (void*)0  // nullptr
  );
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, cboID_);
  glVertexAttribPointer(1,        // Attribute 0 matches our layout for vertex positions
      4,        // Size
      GL_FLOAT, // Type
      GL_FALSE, // Not normalized
      0,        // Stride - no interleaving
      (void*)0  // nullptr
  );
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID_);
  // Render
  // TODO: Change number of indices drawn
  glDrawElements(GL_TRIANGLES, TRIANGLE ? 3 : 6, GL_UNSIGNED_INT, nullptr);
  // ENDTODO
  // Unbind everything
  glDisableVertexAttribArray(0);
  glUseProgram(NULL);
#endif
}
