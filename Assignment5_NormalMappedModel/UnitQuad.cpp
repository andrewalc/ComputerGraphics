#include "UnitQuad.h"

UnitQuad::UnitQuad() : lightPos_(0.5f, 0.5f, -2.0f), sign_(1.0f)
{}

UnitQuad::~UnitQuad()
{}

void UnitQuad::init(const QString& textureFile)
{
	// The unit quad goes from 0.0 to 1.0 in each dimension.
	QVector<QVector3D> pos;
	QVector<QVector3D> norm;
	QVector<QVector2D> texCoord;
	QVector<unsigned int> idx;
    // unit positions.
    pos << QVector3D(0.0, 0.0, 0.0);
    pos << QVector3D(1.0, 0.0, 0.0);
    pos << QVector3D(0.0, 1.0, 0.0);
    pos << QVector3D(1.0, 1.0, 0.0);
    // We use normals for shading and lighting
    norm << QVector3D(0.0, 0.0, 1.0);
    norm << QVector3D(0.0, 0.0, 1.0);
    norm << QVector3D(0.0, 0.0, 1.0);
    norm << QVector3D(0.0, 0.0, 1.0);
    // Add in the texcoords
    texCoord << QVector2D(0.0, 0.0);
    texCoord << QVector2D(1.0, 0.0);
    texCoord << QVector2D(0.0, 1.0);
    texCoord << QVector2D(1.0, 1.0);
    idx << 0 << 1 << 2 << 2 << 1 << 3;
    Renderable::init(pos, norm, texCoord, idx, textureFile);
}

void UnitQuad::update(const qint64 msSinceLastFrame)
{
    // This is where we want to maintain our light.
    float secs = (float)msSinceLastFrame / 1000.0f;
    float angle = secs * 180.0f;
    // Rotate our light around the scene
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.rotate(angle, 0.0, 1.0, 0.0);
    QVector3D newPos = rot * lightPos_;
    lightPos_ = newPos;
    // Because we aren't doing any occlusion, the lighting on the walls looks
    // super wonky.  Instead, just move the light on the z axis.
    newPos.setX(.5);
    newPos.setY(2.0f);
    // TODO:  Understand how the light gets initialized/setup.
    shader_.bind();
    shader_.setUniformValue("pointLights[0].color", 1.0f, 1.0f, 1.0f);
    shader_.setUniformValue("pointLights[0].position", newPos);

    shader_.setUniformValue("pointLights[0].ambientIntensity", 0.0f);
    shader_.setUniformValue("pointLights[0].specularStrength", 0.5f);
    shader_.setUniformValue("pointLights[0].constant", 0.2f);
    shader_.setUniformValue("pointLights[0].linear", 0.1f);
    shader_.setUniformValue("pointLights[0].quadratic", 0.05f);

    newPos.setX(.1);
    shader_.setUniformValue("pointLights[1].color", 0, 0, 1);
    shader_.setUniformValue("pointLights[1].position", newPos);

    shader_.setUniformValue("pointLights[1].ambientIntensity", 0.5f);
    shader_.setUniformValue("pointLights[1].specularStrength", 0.5f);
    shader_.setUniformValue("pointLights[1].constant", .5f);
    shader_.setUniformValue("pointLights[1].linear", 0.09f);
    shader_.setUniformValue("pointLights[1].quadratic", 3.0f);

    newPos.setX(.9);
    shader_.setUniformValue("pointLights[2].color", 0, 1, 0);
    shader_.setUniformValue("pointLights[2].position", newPos);

    shader_.setUniformValue("pointLights[2].ambientIntensity", 0.5f);
    shader_.setUniformValue("pointLights[2].specularStrength", 0.5f);
    shader_.setUniformValue("pointLights[2].constant", .5f);
    shader_.setUniformValue("pointLights[2].linear", 0.09f);
    shader_.setUniformValue("pointLights[2].quadratic", 3.0f);
    shader_.release();
}