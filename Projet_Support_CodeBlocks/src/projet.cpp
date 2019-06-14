#include <cmath>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include "projet.h"

//constructor
Charge::Charge(double _charge, Sphere _sphere, Vector _force, int _bloquage)//,Charge _chargeFictive)
{
    charge = _charge;
    sphere = _sphere;
    force = _force;
    bloquage = _bloquage;
    //chargeFictive = _chargeFictive;
}

//Render de la charge
void Charge::render()
{
    this->sphere.render();
}
