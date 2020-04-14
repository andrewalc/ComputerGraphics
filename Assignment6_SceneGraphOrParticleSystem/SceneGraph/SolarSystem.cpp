#include "SolarSystem.h"

SolarSystem::SolarSystem(void) {

    SceneNode* earth = new SceneNode(new Sphere(), QVector4D(1,0,0,1));
    earth->SetModelScale(QVector3D(5,5,5));
    QMatrix4x4 mat;
    mat.setToIdentity();
    earth->SetTransform(mat);

    //the sun
    planets = new SceneNode(new Sphere(), QVector4D(1,0,0,1));
    planets->SetModelScale(QVector3D(5,5,5));
    QMatrix4x4 mat;
    mat.setToIdentity();
    planets->SetTransform(mat);
    planets->AddChild(earth);
}

void SolarSystem::Update(float msec) {
    planets->Update(msec);
}