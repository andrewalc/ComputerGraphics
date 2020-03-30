#include "BasicWidget.h"

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
    } else {
        qDebug() << "You Pressed an unsupported Key!";
    }
}

void BasicWidget::initializeGL() {
    makeCurrent();
    initializeOpenGLFunctions();

    qDebug() << QDir::currentPath();

    Renderable* house = makeHouse(QVector3D(0, 0, 0));
    // QMatrix4x4 transform;
    // transform.setToIdentity();
    // transform.translate(1,0,0);
    // house->setModelMatrix(transform);
    // house->setRotationAxis(QVector3D(0,1,0));
    // house->setRotationSpeed(7.0f);

    glViewport(0, 0, width(), height());
    frameTimer_.start();
}

Renderable* BasicWidget::makeHouse(QVector3D offset) {

    QString texFile = "./cat3.ppm";
    ObjParse parser = ObjParse();
    parser.parse("objects/house/house_obj.obj");
    PPM texturePPM = PPM("cat3.ppm");
    QVector<QVector3D> pos = parser.getVerts3D();
    QVector<QVector3D> norm;
    QVector<QVector2D> texCoord = parser.getVTData2D();
    QVector<unsigned int> idx =
        QVector<unsigned int>::fromStdVector(parser.getIdx());

    // QVector<unsigned int> idx;
    // pos << QVector3D(-0.8 + offset.x(), -0.8 + offset.y(), 0.0 + offset.z());
    // pos << QVector3D(0.8 + offset.x(), -0.8 + offset.y(), 0.0 + offset.z());
    // pos << QVector3D(-0.8 + offset.x(), 0.8 + offset.y(), 0.0 + offset.z());
    // pos << QVector3D(0.8 + offset.x(), 0.8 + offset.y(), 0.0 + offset.z());
    // We don't actually use the normals right now, but this will be useful
    // later!
    norm << QVector3D(0.0, 0.0, 1.0);
    norm << QVector3D(0.0, 0.0, 1.0);
    norm << QVector3D(0.0, 0.0, 1.0);
    norm << QVector3D(0.0, 0.0, 1.0);
    // TODO:  Make sure to add texture coordinates to pass into the
    // initialization of our renderable idx << 0 << 1 << 2 << 2 << 1 << 3;
    // texCoord << QVector2D(1, 1);
    // texCoord << QVector2D(0, 1);
    // texCoord << QVector2D(1, 0);
    // texCoord << QVector2D(0, 0);

    Renderable* ren = new Renderable();

    ren->init(pos, norm, texCoord, idx, texFile);
    renderables_.push_back(ren);
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
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto renderable : renderables_) {
        renderable->update(msSinceRestart);
        renderable->draw(view_, projection_);
    }

    update();
}
