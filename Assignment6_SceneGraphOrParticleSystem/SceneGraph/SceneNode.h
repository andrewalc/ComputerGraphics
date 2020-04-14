#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#include <vector>

#include "Renderable.h"
#include "Camera.h"

class SceneNode {
   public:
    SceneNode(Renderable* m = NULL, QVector4D color = QVector4D(1, 1, 1, 1));
    ~SceneNode(void);

    void SetTransform(const QMatrix4x4& matrix) { localTransform = matrix; }
    const QMatrix4x4 GetTransform() const { return localTransform; }
    QMatrix4x4 GetWorldTransform() const { return worldTransform; }
    QVector4D GetColor() const { return color; }
    void SetColor(QVector4D c) { color = c; }
    QVector3D GetModelScale() const  { return modelScale; }
    void SetModelScale (QVector3D s) { modelScale = s; }
    Renderable* GetMesh() const { return object; }
    void SetMesh(Renderable* m) { object = m; } 
    void AddChild(SceneNode* s);
    virtual void Update(float msec);
    virtual void Draw(QMatrix4x4 world_, Camera camera_);
    std::vector<SceneNode*>::const_iterator GetChildIteratorStart() { return children.begin(); }
    std::vector<SceneNode*>::const_iterator GetChildIteratorEnd() { return children.end(); }

   protected:
    // Parent
    SceneNode* parent;
    // The object stored in the scene graph
    Renderable* object;  // May be nullptr!
    // We additionally can store the world transform
    QMatrix4x4 worldTransform;
    // Each SceneNode nodes locals transform.
    QMatrix4x4 localTransform;
    QVector3D modelScale;
    QVector4D color;
    // Children holds all a pointer to all of the descendents
    // of a particular SceneNode. A pointer is used because
    // we do not want to hold or make actual copies.
    std::vector<SceneNode*> children;
};