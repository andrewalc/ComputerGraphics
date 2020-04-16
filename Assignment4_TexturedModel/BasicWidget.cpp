#include "BasicWidget.h"
bool WIREFRAME = false;

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent)
    : QOpenGLWidget(parent), logger_(this) {
    setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget() {
    for (auto renderable : renderables_) {
        delete renderable;
    }
    renderables_.clear();
}

//////////////////////////////////////////////////////////////////////
// Privates
///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent) {
    // Handle key events here.
    if (keyEvent->key() == Qt::Key_Left) {
        qDebug() << "Left Arrow Pressed";
        update();  // We call update after we handle a key press to trigger a
                   // redraw when we are ready
    } else if (keyEvent->key() == Qt::Key_Right) {
        qDebug() << "Right Arrow Pressed";
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

void BasicWidget::initializeGL() {
    makeCurrent();
    initializeOpenGLFunctions();

    qDebug() << QDir::currentPath();

    Renderable* house = makeObject("objects/house/house_obj.obj", "objects/house/house_diffuse.ppm", QVector3D(0, 0, 0));
    glViewport(0, 0, width(), height());
    frameTimer_.start();
}

Renderable* BasicWidget::makeObject(std::string objFile, std::string texFile, QVector3D offset) {
    // ./App"objects/house/house_obj.obj" "objects/house/house_diffuse.ppm"
    ObjParse parser = ObjParse();
    parser.parse(objFile);

    QVector<unsigned int> idx = QVector<unsigned int>::fromStdVector(parser.getIdx());

    Renderable* ren = new Renderable();
    ren->init(parser.vertextures, idx, QString::fromStdString(texFile));
    renderables_.push_back(ren);
    std::cout << "object " <<  objFile << " created " << std::endl;
    return ren;
}

void BasicWidget::resizeGL(int w, int h) {
    if (!logger_.isLogging()) {
        logger_.initialize();
        // Setup the logger for real-time messaging
        connect(&logger_, &QOpenGLDebugLogger::messageLogged, [=]() {
            const QList<QOpenGLDebugMessage> messages =
                logger_.loggedMessages();
            for (auto msg : messages) {
                qDebug() << msg;
            }
        });
        logger_.startLogging();
    }
    glViewport(0, 0, w, h);
    view_.setToIdentity();
    view_.lookAt(QVector3D(0.0f, 0.0f, 2.0f), QVector3D(0.0f, 0.0f, 0.0f),
                 QVector3D(0.0f, 1.0f, 0.0f));
    projection_.setToIdentity();
    projection_.perspective(70.f, (float)w / (float)h, 0.001, 1000.0);
    glViewport(0, 0, w, h);
}

void BasicWidget::paintGL() {
    qint64 msSinceRestart = frameTimer_.restart();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto renderable : renderables_) {
        renderable->update(msSinceRestart);
        renderable->draw(view_, projection_);
    }

    update();
}
