#include "SceneNode.h"
#include <iostream>
SceneNode::SceneNode(Renderable* m, QVector4D color) {

    this-> object = m;
    this->color = color;
    parent = NULL;
    modelScale = QVector3D(1,1,1);
}

SceneNode::~SceneNode(void) {
    for(unsigned int i = 0; i < children.size(); ++i) {
        delete children[i];
    }
}

void SceneNode::AddChild(SceneNode* s) {
    children.push_back(s);
    s->parent = this;
}

void SceneNode::Draw(QMatrix4x4 world_, Camera camera_) {
    if(object) {
        object->draw(worldTransform, camera_.getViewMatrix(), camera_.getProjectionMatrix());
    }
}

void SceneNode::Update(float msec) {
    if(parent) {
        // orbit
        worldTransform = parent->worldTransform * localTransform;
    }
    else {
        worldTransform = localTransform;
    }
    for(std::vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
        (*i)->Update(msec);
    }

}