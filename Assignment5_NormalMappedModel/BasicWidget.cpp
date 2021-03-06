#include "BasicWidget.h"

#include "ObjParse.h"
#include "UnitQuad.h"
bool WIREFRAME = false;
//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent)
    : QOpenGLWidget(parent), logger_(this) {
    setFocusPolicy(Qt::StrongFocus);
    camera_.setPosition(QVector3D(0.5, 0.5, -2.0));
    camera_.setLookAt(QVector3D(0.5, 0.5, 0.0));
    world_.setToIdentity();
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
    } else if (keyEvent->key() == Qt::Key_R) {
        camera_.setPosition(QVector3D(0.5, 0.5, -2.0));
        camera_.setLookAt(QVector3D(0.5, 0.5, 0.0));
        update();
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
    } else {
        qDebug() << "You Pressed an unsupported Key!";
    }
}

void BasicWidget::mousePressEvent(QMouseEvent* mouseEvent) {
    if (mouseEvent->button() == Qt::LeftButton) {
        mouseAction_ = Rotate;
    } else if (mouseEvent->button() == Qt::RightButton) {
        mouseAction_ = Zoom;
    }
    lastMouseLoc_ = mouseEvent->pos();
}

void BasicWidget::mouseMoveEvent(QMouseEvent* mouseEvent) {
    if (mouseAction_ == NoAction) {
        return;
    }
    QPoint delta = mouseEvent->pos() - lastMouseLoc_;
    lastMouseLoc_ = mouseEvent->pos();
    if (mouseAction_ == Rotate) {
        // TODO:  Implement rotating the camera
        // QVector3D gaze = camera_.gazeVector();
        // camera_.translateCamera(gaze + QVector(gaze.x() + 1 , gaze.y() + 1,
        // gaze.z()) * camera_.ROTATION_SPEED);
        // camera_.translateLookAt(camera_.upVector() * camera_.ROTATION_SPEED
        //  * (delta.y() > 0 ? -1 : 1));
        camera_.translateLookAt(
            QVector3D(0, 0, 0).crossProduct(
                camera_.lookAt() - camera_.position(), camera_.upVector()) *
            camera_.ROTATION_SPEED * delta.x());
    } else if (mouseAction_ == Zoom) {
        // TODO:  Implement zoom by moving the camera
        // Zooming is moving along the gaze direction by some amount.
        camera_.translateCamera(camera_.gazeVector() * camera_.ZOOM_SPEED *
                                -delta.y());
    }
    update();
}

void BasicWidget::mouseReleaseEvent(QMouseEvent* mouseEvent) {
    mouseAction_ = NoAction;
}

Renderable* BasicWidget::makeObject(std::string objFile, std::string texFile,
                                    QVector3D offset) {
    // ./App"objects/house/house_obj.obj" "objects/house/house_diffuse.ppm"
    ObjParse parser = ObjParse();
    parser.parse(objFile);

    QVector<unsigned int> idx =
        QVector<unsigned int>::fromStdVector(parser.getIdx());

    Renderable* ren = new UnitQuad();
    ren->init(parser.vertextures, idx, QString::fromStdString(texFile));
    QMatrix4x4 backXform;
    backXform.setToIdentity();
    backXform.translate(2.8, .55, 1.0);
    backXform.rotate(QQuaternion::fromEulerAngles(QVector3D(0,260,0)));
    ren->setModelMatrix(backXform);
    renderables_.push_back(ren);
    std::cout << "object " <<  objFile << " created " << std::endl;
    return ren;
}

void BasicWidget::initializeGL() {
    makeCurrent();
    initializeOpenGLFunctions();

    qDebug() << QDir::currentPath();
    Renderable* house = makeObject("objects/house/house_obj.obj", "objects/house/house_diffuse.ppm", QVector3D(0, 0, 0));

    // TODO:  You may have to change these paths.
    QString brickTex = "../objects/brickWall_highRes/brickWall_diffuse.ppm";
    QString grassTex = "./grass.ppm";

    UnitQuad* backWall = new UnitQuad();
    backWall->init(brickTex);
    QMatrix4x4 backXform;
    backXform.setToIdentity();
    backXform.scale(1.0, 1.0, -1.0);
    backWall->setModelMatrix(backXform);
    renderables_.push_back(backWall);

    UnitQuad* rightWall = new UnitQuad();
    rightWall->init(brickTex);
    QMatrix4x4 rightXform;
    rightXform.setToIdentity();
    rightXform.rotate(90.0, 0.0, 1.0, 0.0);
    rightWall->setModelMatrix(rightXform);
    renderables_.push_back(rightWall);

    // UnitQuad* leftWall = new UnitQuad();
    // leftWall->init(brickTex);
    // QMatrix4x4 leftXform;
    // leftXform.setToIdentity();
    // leftXform.translate(1.0, 0.0, -1.0);
    // leftXform.rotate(-90.0, 0.0, 1.0, 0.0);
    // leftWall->setModelMatrix(leftXform);
    // renderables_.push_back(leftWall);

    UnitQuad* floor = new UnitQuad();
    floor->init(grassTex);
    QMatrix4x4 floorXform;
    floorXform.setToIdentity();
    floorXform.translate(-0.5, 0.0, 0.5);
    floorXform.scale(2.0, 2.0, 2.0);
    floorXform.rotate(-90.0, 1.0, 0.0, 0.0);
    floor->setModelMatrix(floorXform);
    renderables_.push_back(floor);

    glViewport(0, 0, width(), height());
    frameTimer_.start();
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

    camera_.setPerspective(70.f, (float)w / (float)h, 0.001, 1000.0);
    glViewport(0, 0, w, h);
}

void BasicWidget::paintGL() {
    qint64 msSinceRestart = frameTimer_.restart();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    for (auto renderable : renderables_) {
        renderable->update(msSinceRestart);
        // TODO:  Understand that the camera is now governing the view and
        // projection matrices
        renderable->draw(world_, camera_.getViewMatrix(),
                         camera_.getProjectionMatrix());
    }
    update();
}
