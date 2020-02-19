#include "Lab5Widget.h"

static const std::vector<float> vertsO{-0.8f, -0.8f, 0.0f, 0.8f, -0.8f, 0.0f,
                                       -0.8f, 0.8f,  0.0f, 0.8f, 0.8f,  0.0f};

static const std::vector<int> idxO{0, 1, 2, 2, 1, 3};

// set global toggles
bool BUNNY = true;
bool WIREFRAME = false;
#if USE_QT_OPENGL
Lab5Widget::Lab5Widget(QWidget* parent)
    : QOpenGLWidget(parent),
      vbo_(QOpenGLBuffer::VertexBuffer),
      ibo_(QOpenGLBuffer::IndexBuffer)
#else
Lab5Widget::Lab5Widget(QWidget* parent)
    : QOpenGLWidget(parent)
#endif
{
    setFocusPolicy(Qt::StrongFocus);
    // Load Bunny
    ObjParse bunnyParse = ObjParse();
    bunnyParse.parse("objects/bunny_centered.obj");
    bunnyVerts = bunnyParse.verts;
    bunnyVertNormals = bunnyParse.vertNormals;
    bunnyFaces = bunnyParse.faces;
    // Load Monkey
    ObjParse monkeyParse = ObjParse();
    monkeyParse.parse("objects/monkey_centered.obj");
    monkeyVerts = monkeyParse.verts;
    monkeyVertNormals = monkeyParse.vertNormals;
    monkeyFaces = monkeyParse.faces;
}

void Lab5Widget::releaseBuffers() {
    vbo_.release();
    ibo_.release();
    vao_.release();
}

Lab5Widget::~Lab5Widget() {
#if USE_QT_OPENGL
    releaseBuffers();
    vbo_.destroy();
    ibo_.destroy();
    vao_.destroy();
#else
    glDeleteProgram(shaderID_);
    glDeleteBuffers(1, &vboID_);
    glDeleteBuffers(1, &iboID_);
#endif
}

QString Lab5Widget::vertexShaderString() const {
    QString str =
        "#version 330\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec4 color;\n"

        "uniform mat4 modelMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 projectionMatrix;\n"

        "out vec4 vertColor;\n"

        "void main()\n"
        "{\n"
        "  mat4 MVPmatrix = projectionMatrix * viewMatrix * modelMatrix;\n"

        "  gl_Position = vec4(position, 1.0);\n"
        "  gl_Position = MVPmatrix * gl_Position;\n"
        "  vertColor = color;\n"
        "}\n";
    return str;
}

QString Lab5Widget::fragmentShaderString() const {
    QString str =
        "#version 330\n"
        "in vec4 vertColor;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "  color = vec4(0.43137f, 0.64314f, 1.0f, 1.0f);\n"
        "}\n";
    return str;
}

#if USE_QT_OPENGL
void Lab5Widget::createShader() {
    QOpenGLShader vert(QOpenGLShader::Vertex);
    vert.compileSourceCode(vertexShaderString());
    QOpenGLShader frag(QOpenGLShader::Fragment);
    frag.compileSourceCode(fragmentShaderString());
    bool ok = shaderProgram_.addShader(&vert);
    if (!ok) {
        qDebug() << shaderProgram_.log();
    }
    ok = shaderProgram_.addShader(&frag);
    if (!ok) {
        qDebug() << shaderProgram_.log();
    }
    ok = shaderProgram_.link();
    if (!ok) {
        qDebug() << shaderProgram_.log();
    }
}
#else
void Lab5Widget::createShader() {
    unsigned int prog = glCreateProgram();
    unsigned int vert = compileShader(GL_VERTEX_SHADER, vertexShaderString());
    unsigned int frag =
        compileShader(GL_FRAGMENT_SHADER, fragmentShaderString());
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);
    glValidateProgram(prog);

    // The shaders are build and ready to use.  We can now detach the individual
    // shaders and destroy them
    glDetachShader(prog, vert);
    glDetachShader(prog, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

    // Assign here in case there are failures we need to bail on - This has not
    // yet been implemented, though.
    shaderID_ = prog;
}

unsigned int Lab5Widget::compileShader(unsigned int type,
                                       const QString& shaderSrc) {
    unsigned int id = glCreateShader(type);
    // Make sure we can create the shader
    if (id == 0) {
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
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];  // Could also use alloca here.
        glGetShaderInfoLog(id, length, &length, errorMessages);
        if (type == GL_VERTEX_SHADER) {
            qDebug() << "ERROR: GL_VERTEX_SHADER compilation failed!\n"
                     << errorMessages << "\n";
        } else if (type == GL_FRAGMENT_SHADER) {
            qDebug() << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n"
                     << errorMessages << "\n";
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

// Key handler. Checks for keys pressed and acts accordingly.
void Lab5Widget::keyReleaseEvent(QKeyEvent* keyEvent) {
    if (keyEvent->key() == Qt::Key_1) {
        qDebug() << "Left Arrow Pressed";
        BUNNY = true;
        releaseBuffers();
        loadData(bunnyVerts, ObjParse::getIdx(bunnyFaces));
        update();  // We call update after we handle a key press to trigger a
                   // redraw when we are ready
    } else if (keyEvent->key() == Qt::Key_2) {
        qDebug() << "Right Arrow Pressed";
        BUNNY = false;
        releaseBuffers();
        loadData(monkeyVerts, ObjParse::getIdx(monkeyFaces));
        update();  // We call update after we handle a key press to trigger a
                   // redraw when we are ready
    } else if (keyEvent->key() == Qt::Key_W) {
        qDebug() << "W Pressed";
        makeCurrent();

        WIREFRAME = !WIREFRAME;
        if (WIREFRAME) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        update();  // We call update after we handle a key press to trigger a
                   // redraw when we are ready
    } else if (keyEvent->key() == Qt::Key_Q) {
        qDebug() << "Q Pressed";
        exit(0);
    } else {
        qDebug() << "You Pressed an unsupported Key!";
    }
}

// load in the verts and the idx (faces) and bind and allocate them
void Lab5Widget::loadData(std::vector<float> verts, std::vector<int> idx) {
    QVector<float> qVert = QVector<float>::fromStdVector(verts);
    QVector<int> qIdx = QVector<int>::fromStdVector(idx);

    releaseBuffers();
    // Temporary bind of our shader.
    shaderProgram_.bind();

    // Bind our vbo inside our vao
    vbo_.bind();
    vbo_.allocate(qVert.constData(), qVert.size() * sizeof(float));

    // Bind our vbo inside our vao
    ibo_.bind();
    ibo_.allocate(qIdx.constData(), qIdx.size() * sizeof(float));

    vao_.bind();
    vbo_.bind();
    shaderProgram_.enableAttributeArray(0);
    shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3);

    ibo_.bind();
    // Release the vao THEN the vbo
    vao_.release();
    shaderProgram_.release();
}

void Lab5Widget::initializeGL() {
    makeCurrent();
    initializeOpenGLFunctions();

    QOpenGLContext* curContext = this->context();
    qDebug() << "[Lab5Widget]::initializeGL() -- Context Information:";
    qDebug() << "  Context Valid: "
             << std::string(curContext->isValid() ? "true" : "false").c_str();
    qDebug() << "  GL Version Used: " << curContext->format().majorVersion()
             << "." << curContext->format().minorVersion();
    qDebug() << "  Vendor: "
             << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    qDebug() << "  Renderer: "
             << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    qDebug() << "  Version: "
             << reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << "  GLSL Version: "
             << reinterpret_cast<const char*>(
                    glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Set up our shaders.
    createShader();
    // All of our matrices should be set to identity for now.
    model_.setToIdentity();
    view_.setToIdentity();
    projection_.setToIdentity();

    // Set up our buffers and our vao
#if USE_QT_OPENGL
    // Create and prepare a vbo
    vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo_.create();
    // Generate our index buffer
    ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo_.create();
    // Create a VAO to keep track of things for us.
    vao_.create();
#else
    vao_.create();
    vao_.bind();

    glGenBuffers(1, &vboID_);
    glBindBuffer(GL_ARRAY_BUFFER, vboID_);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GL_FLOAT), verts, GL_STATIC_DRAW);

    // Generate our index buffer
    glGenBuffers(1, &ibo_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GL_FLOAT), idx,
                 GL_STATIC_DRAW);

    vao_.release();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
    glViewport(0, 0, width(), height());

    loadData(bunnyVerts, ObjParse::getIdx(bunnyFaces));
}
// create our window
void Lab5Widget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    shaderProgram_.bind();
    model_.rotate(100.0f * 10 / screen()->refreshRate(), 0, 1, 0);
    shaderProgram_.setUniformValue("modelMatrix", model_);
    shaderProgram_.setUniformValue("viewMatrix", view_);
    shaderProgram_.setUniformValue("projectionMatrix", projection_);
    shaderProgram_.release();
}

void Lab5Widget::paintGL() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.15f, 0.15f, 0.15f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if USE_QT_OPENGL
    shaderProgram_.bind();
    vao_.bind();
    glDrawElements(GL_TRIANGLES,
                   (BUNNY ? bunnyFaces.size() : monkeyFaces.size()) * 1.5f,
                   GL_UNSIGNED_INT, 0);
    vao_.release();
    shaderProgram_.release();
#else
    glUseProgram(shaderID_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vboID_);
    glVertexAttribPointer(
        0,         // Attribute 0 matches our layout for vertex positions
        3,         // Size
        GL_FLOAT,  // Type
        GL_FALSE,  // Not normalized
        0,         // Stride - no interleaving
        (void*)0   // nullptr
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,         // Attribute 0 matches our layout for vertex positions
        4,         // Size
        GL_FLOAT,  // Type
        GL_FALSE,  // Not normalized
        0,         // Stride - no interleaving
        (void*)0   // nullptr
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID_);
    // Render
    glDrawElements(GL_TRIANGLES, TRIANGLE ? 3 : 6, GL_UNSIGNED_INT, nullptr);
    // Unbind everything
    glDisableVertexAttribArray(0);
    glUseProgram(NULL);
#endif
}