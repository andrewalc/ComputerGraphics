/** @file Sphere.h
 *  @brief Draw a simple sphere primitive.
 *
 *  Draws a simple sphere primitive, that is derived
 *  from the Object class.
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#include <QtGui>
#include <cmath>
#include <iostream>
#include "Renderable.h"

class Sphere : public Renderable {
   public:
    // Constructor for the Sphere
    Sphere(QString textureFile);
    // The intialization routine for this object.
	// Our init method is much easier now.  We only need a texture!
	virtual void init(const QString& textureFile);
	virtual void update(const qint64 msSinceLastFrame) override;

    // Getters for our data.
    QVector<QVector3D> positions() const { return positions_; }
    QVector<QVector3D> normals() const { return normals_; }
    QVector<QVector2D> texCoords() const { return textureCoords_; }
    QVector<unsigned int> indexes() const { return index_; }

   private:
    QVector<QVector3D> positions_;
    QVector<QVector3D> normals_;
    QVector<QVector2D> textureCoords_;
    QVector<unsigned int> index_;
};

// Calls the initalization routine
Sphere::Sphere(QString textureFile) {
    std::cout
        << "(Sphere.cpp) Sphere constructor called (derived from Object)\n";
    init(textureFile);
}


// Algorithm for rendering a sphere
// The algorithm was obtained here: http://learningwebgl.com/blog/?p=1253
// Please review the page so you can understand the algorithm. You may think
// back to your algebra days and equation of a circle! (And some trig with
// how sin and cos work
void Sphere::init(const QString& textureFile) {
    unsigned int latitudeBands = 30;
    unsigned int longitudeBands = 30;
    float radius = 1.0f;
    double PI = 3.14159265359;

    for (unsigned int latNumber = 0; latNumber <= latitudeBands; latNumber++) {
        float theta = latNumber * PI / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (unsigned int longNumber = 0; longNumber <= longitudeBands;
             longNumber++) {
            float phi = longNumber * 2 * PI / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;
            // Why is this "1-" Think about the range of texture coordinates
            float u = 1 - ((float)longNumber / (float)longitudeBands);
            float v = 1 - ((float)latNumber / (float)latitudeBands);

            // Setup geometry
            positions_.push_back(
                QVector3D(radius * x, radius * y, radius * z));  // Position
            normals_.push_back(QVector3D(radius * x, radius * y, radius * z));
            textureCoords_.push_back(QVector2D(u, v));  // Texture
        }
    }

    // Now that we have all of our vertices
    // generated, we need to generate our indices for our
    // index element buffer.
    // This diagram shows it nicely visually
    // http://learningwebgl.com/lessons/lesson11/sphere-triangles.png
    for (unsigned int latNumber1 = 0; latNumber1 < latitudeBands;
         latNumber1++) {
        for (unsigned int longNumber1 = 0; longNumber1 < longitudeBands;
             longNumber1++) {
            unsigned int first =
                (latNumber1 * (longitudeBands + 1)) + longNumber1;
            unsigned int second = first + longitudeBands + 1;
            index_.push_back(first);
            index_.push_back(second);
            index_.push_back(first + 1);

            index_.push_back(second);
            index_.push_back(second + 1);
            index_.push_back(first + 1);
        }
    }
    Renderable::init(positions_, normals_, textureCoords_, index_, textureFile);

}

void Sphere::update(const qint64 msSinceLastFrame)
{
    // // This is where we want to maintain our light.
    // float secs = (float)msSinceLastFrame / 1000.0f;
    // float angle = secs * 180.0f;
    // // Rotate our light around the scene
    // QMatrix4x4 rot;
    // rot.setToIdentity();
    // rot.rotate(angle, 0.0, 1.0, 0.0);
    // QVector3D newPos = rot * lightPos_;
    // lightPos_ = newPos;
    // // Because we aren't doing any occlusion, the lighting on the walls looks
    // // super wonky.  Instead, just move the light on the z axis.
    // newPos.setX(.5);
    // // TODO:  Understand how the light gets initialized/setup.
    // shader_.bind();
    // shader_.setUniformValue("pointLights[0].color", 1.0f, 0.0f, 0.0f);
    // shader_.setUniformValue("pointLights[0].position", newPos);

    // shader_.setUniformValue("pointLights[0].ambientIntensity", 0.0f);
    // shader_.setUniformValue("pointLights[0].specularStrength", 0.5f);
    // shader_.setUniformValue("pointLights[0].constant", 0.2f);
    // shader_.setUniformValue("pointLights[0].linear", 0.3f);
    // shader_.setUniformValue("pointLights[0].quadratic", 1.0f);

    // newPos.setX(.1);
    // shader_.setUniformValue("pointLights[1].color", 0, 0, 1);
    // shader_.setUniformValue("pointLights[1].position", newPos);

    // shader_.setUniformValue("pointLights[1].ambientIntensity", 0.5f);
    // shader_.setUniformValue("pointLights[1].specularStrength", 0.5f);
    // shader_.setUniformValue("pointLights[1].constant", .5f);
    // shader_.setUniformValue("pointLights[1].linear", 0.09f);
    // shader_.setUniformValue("pointLights[1].quadratic", 3.0f);

    // newPos.setX(.9);
    // shader_.setUniformValue("pointLights[2].color", 0, 1, 0);
    // shader_.setUniformValue("pointLights[2].position", newPos);

    // shader_.setUniformValue("pointLights[2].ambientIntensity", 0.5f);
    // shader_.setUniformValue("pointLights[2].specularStrength", 0.5f);
    // shader_.setUniformValue("pointLights[2].constant", .5f);
    // shader_.setUniformValue("pointLights[2].linear", 0.09f);
    // shader_.setUniformValue("pointLights[2].quadratic", 3.0f);
    // shader_.release();
}
