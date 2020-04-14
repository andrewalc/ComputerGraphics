#include "SceneNode.h"
#include "Sphere.h"

class SolarSystem {
   public:
    SolarSystem(void);
    ~SolarSystem(void);
    virtual void Update(float msec);

    // static void CreatePlanet() {
    //     Sphere* planet = new Sphere();
    //     planet->init();
    // }

   protected:
    SceneNode* planets;
}