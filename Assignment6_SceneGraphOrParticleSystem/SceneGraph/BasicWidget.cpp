#include "BasicWidget.h"
#include "UnitQuad.h"
#include "Sphere.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent), logger_(this)
{
  setFocusPolicy(Qt::StrongFocus);
  camera_.setPosition(QVector3D(0.5, 0.5, -2.0));
  camera_.setLookAt(QVector3D(0.5, 0.5, 0.0));
  world_.setToIdentity();
}

BasicWidget::~BasicWidget()
{
    for (auto renderable : renderables_) {
        delete renderable;
    }
    renderables_.clear();
    delete root;
}

//////////////////////////////////////////////////////////////////////
// Privates
///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  // Handle key events here.
  if (keyEvent->key() == Qt::Key_Left) {
    qDebug() << "Left Arrow Pressed";
    update();  // We call update after we handle a key press to trigger a redraw when we are ready
  } else if (keyEvent->key() == Qt::Key_Right) {
    qDebug() << "Right Arrow Pressed";
    update();  // We call update after we handle a key press to trigger a redraw when we are ready
  } else if (keyEvent->key() == Qt::Key_R) {
    camera_.setPosition(QVector3D(0.5, 0.5, -2.0));
    camera_.setLookAt(QVector3D(0.5, 0.5, 0.0));
    update();
  } else {
    qDebug() << "You Pressed an unsupported Key!";
  }
}

void BasicWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
  if (mouseEvent->button() == Qt::LeftButton) {
    mouseAction_ = Rotate;
  } else if (mouseEvent->button() == Qt::RightButton) {
    mouseAction_ = Zoom;
  }
  lastMouseLoc_ = mouseEvent->pos();
}

void BasicWidget::mouseMoveEvent(QMouseEvent* mouseEvent)
{
  if (mouseAction_ == NoAction) {
    return;
  }
  QPoint delta = mouseEvent->pos() - lastMouseLoc_;
  lastMouseLoc_ = mouseEvent->pos();
  if (mouseAction_ == Rotate) {
    // TODO:  Implement rotating the camera
    // QVector3D gaze = camera_.gazeVector();
    // camera_.translateCamera(gaze + QVector(gaze.x() + 1 , gaze.y() + 1, gaze.z()) * camera_.ROTATION_SPEED);
    // camera_.translateLookAt(camera_.upVector() * camera_.ROTATION_SPEED
    //  * (delta.y() > 0 ? -1 : 1));
    camera_.translateLookAt(QVector3D(0,0,0).crossProduct(camera_.lookAt() - camera_.position(), camera_.upVector()) * camera_.ROTATION_SPEED
    * delta.x());
  } else if (mouseAction_ == Zoom) {
    // TODO:  Implement zoom by moving the camera
    // Zooming is moving along the gaze direction by some amount.
      camera_.translateCamera(camera_.gazeVector() * camera_.ZOOM_SPEED * -delta.y());
  } 
  update();
}

void BasicWidget::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    mouseAction_ = NoAction;
}

void BasicWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();

  qDebug() << QDir::currentPath();
  // TODO:  You may have to change these paths.
  QString brickTex = "./brick.ppm";
  QString grassTex = "./grass.ppm";

  // UnitQuad* backWall = new UnitQuad();
  // backWall->init(brickTex);
  // QMatrix4x4 backXform;
  // backXform.setToIdentity();
  // backXform.scale(1.0, 1.0, -1.0);
  // backWall->setModelMatrix(backXform);
  // renderables_.push_back(backWall);

  // UnitQuad* rightWall = new UnitQuad();
  // rightWall->init(brickTex);
  // QMatrix4x4 rightXform;
  // rightXform.setToIdentity();
  // rightXform.rotate(90.0, 0.0, 1.0, 0.0);
  // rightWall->setModelMatrix(rightXform);
  // renderables_.push_back(rightWall);

  // UnitQuad* leftWall = new UnitQuad();
  // leftWall->init(brickTex);
  // QMatrix4x4 leftXform;
  // leftXform.setToIdentity();
  // leftXform.translate(1.0, 0.0, -1.0);
  // leftXform.rotate(-90.0, 0.0, 1.0, 0.0);
  // leftWall->setModelMatrix(leftXform);
  // renderables_.push_back(leftWall);

  // UnitQuad* floor = new UnitQuad();
  // floor->init(grassTex);
  // QMatrix4x4 floorXform;
  // floorXform.setToIdentity();
  // floorXform.translate(-0.5, 0.0, 0.5);
  // floorXform.scale(2.0, 2.0, 2.0);
  // floorXform.rotate(-90.0, 1.0, 0.0, 0.0);
  // floor->setModelMatrix(floorXform);
  // renderables_.push_back(floor);
  QString earthTex = "./earth.ppm";
  QString sunTex = "./sun.ppm"; 
  QString mercTex = "./mercury.ppm";
  QString moonTex = "./rock.ppm";
  QString planetTex = "./planet.ppm";

  //the jupiter moon
  Sphere* moon3Sphere = new Sphere(moonTex);
  renderables_.push_back(moon3Sphere);
  SceneNode* moon3 = new SceneNode(moon3Sphere, QVector4D(1,0,0,1));
  moon3->SetModelScale(QVector3D(0.5f,0.5f,0.5f));
  QMatrix4x4 mat;
  mat.setToIdentity();
  mat.translate(QVector3D(14,0,0));
  moon3->SetTransform(mat);

  //the jupiter
  Sphere* planetSphere = new Sphere(planetTex);
  renderables_.push_back(planetSphere);
  SceneNode* planet = new SceneNode(planetSphere, QVector4D(1,0,0,1));
  planet->SetModelScale(QVector3D(0.5f,0.5f,0.5f));
  mat.setToIdentity();
  mat.translate(QVector3D(15,0,0));
  planet->SetTransform(mat);
  planet->AddChild(moon3);


  //the earth moon
  Sphere* moonSphere = new Sphere(moonTex);
  renderables_.push_back(moonSphere);
  SceneNode* moon = new SceneNode(moonSphere, QVector4D(1,0,0,1));
  moon->SetModelScale(QVector3D(0.5f,0.5f,0.5f));
  mat.setToIdentity();
  mat.translate(QVector3D(3,0,0));
  moon->SetTransform(mat);

  //the earth
  Sphere* earthSphere = new Sphere(earthTex);
  renderables_.push_back(earthSphere);
  SceneNode* earth = new SceneNode(earthSphere, QVector4D(1,0,0,1));
  earth->SetModelScale(QVector3D(1,1,1));
  mat.setToIdentity();
  mat.translate(QVector3D(10,0,0));
  earth->SetTransform(mat);
  earth->AddChild(moon);

  //the mercury moon
  Sphere* moon2Sphere = new Sphere(moonTex);
  renderables_.push_back(moon2Sphere);
  SceneNode* moon2 = new SceneNode(moon2Sphere, QVector4D(1,0,0,1));
  moon2->SetModelScale(QVector3D(0.5f,0.5f,0.5f));
  mat.setToIdentity();
  mat.translate(QVector3D(2,0,0));
  moon2->SetTransform(mat);

  //the mercury
  Sphere* mercSphere = new Sphere(mercTex);
  renderables_.push_back(mercSphere);
  SceneNode* mercury = new SceneNode(mercSphere, QVector4D(1,0,0,1));
  mercury->SetModelScale(QVector3D(2,2,2));
  mat.setToIdentity();
  mat.translate(QVector3D(3,0,0));
  mercury->SetTransform(mat);
  mercury->AddChild(moon2);

  //the sun
  Sphere* sunSphere = new Sphere(sunTex);
  renderables_.push_back(sunSphere);
  root = new SceneNode(sunSphere, QVector4D(1,0,0,1));
  root->SetTransform(mat);
  root->AddChild(earth);
  root->AddChild(mercury);
  root->AddChild(planet);
  glViewport(0, 0, width(), height());
  frameTimer_.start();


}

void BasicWidget::resizeGL(int w, int h)
{
    if (!logger_.isLogging()) {
        logger_.initialize();
        // Setup the logger for real-time messaging
        connect(&logger_, &QOpenGLDebugLogger::messageLogged, [=]() {
            const QList<QOpenGLDebugMessage> messages = logger_.loggedMessages();
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

void BasicWidget::paintGL()
{
  qint64 msSinceRestart = frameTimer_.restart();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  for (auto renderable : renderables_) {
      renderable->update(msSinceRestart);
      // TODO:  Understand that the camera is now governing the view and projection matrices
      //renderable->draw(world_, camera_.getViewMatrix(), camera_.getProjectionMatrix());
  }

  

  root->Update(msSinceRestart);
  DrawNode(root, msSinceRestart);
  update();
}

void BasicWidget::DrawNode(SceneNode* n, float msSinceRestart) {
  if (n->GetMesh()) {
    QMatrix4x4 transform = n->GetWorldTransform();
    transform.scale(n->GetModelScale());

    QMatrix4x4 mat = n->GetTransform();
    mat.rotate(-msSinceRestart / 10.0f, QVector3D(0,1,0));
    n->SetTransform(mat);
    n->Draw(world_, camera_);
    for(std::vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
      DrawNode(*i, msSinceRestart);
    }
  }
}