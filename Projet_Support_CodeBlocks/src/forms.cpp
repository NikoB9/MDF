#include <cmath>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include "forms.h"


using namespace std;


void Form::update(double delta_t)
{
    // Nothing to do here, animation update is done in child class method
}


void Form::render()
{
    // Point of view for rendering
    // Common for all Forms
    Point org = anim.getPos();
    glTranslated(org.x, org.y, org.z);
    glColor3f(col.r, col.g, col.b);
}


Sphere::Sphere(double r, Color cl, Point _org)
{
    radius = r;
    col = cl;
    pos = _org;
}


void Sphere::update(double delta_t)
{
    // Complete this part
}


void Sphere::render()
{

    GLUquadric* params;
    params = gluNewQuadric();

    //composition des params de la sphère
    gluQuadricDrawStyle(params,GLU_FILL);
    glColor3f(this->col.r, this->col.g, this->col.b);
    glTranslated(this->pos.x,this->pos.y,this->pos.z);

    //Rendu de la sphere
    gluSphere(params,this->getRadius(),110,110);
    gluDeleteQuadric(params);

}


Cube_face::Cube_face(Vector v1, Vector v2, Point org, double l, double w, Color cl)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    width = w;
    col = cl;
}


void Cube_face::update(double delta_t)
{
    // Do nothing, no physics associated to a Cube_face
}


void Cube_face::render()
{
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length*vdir1);
    p3 = p2;
    p3.translate(width*vdir2);
    p4.translate(width*vdir2);

    Form::render();
    glBegin(GL_QUADS);
    {
        glColor3f(1,1,0);
        glVertex3d(p1.x, p1.y, p1.z);
        glColor3f(0,1,1);
        glVertex3d(p2.x, p2.y, p2.z);
        glColor3f(1,0,1);
        glVertex3d(p3.x, p3.y, p3.z);
        glColor3f(0,1,0);
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();
}


//constructor
Charge::Charge(double _charge, Sphere _sphere, Vector _force, int _bloquage)
{
    chargeValue = _charge;
    sphere = _sphere;
    force = _force;
    bloquage = _bloquage;
   // chargeFictive = _chargeFictive;
}

//Render de la charge
void Charge::render()
{
    this->sphere.render();
}

void Charge::update(double delta_t)
{
    // Nothing to do here, animation update is done in child class method
}
