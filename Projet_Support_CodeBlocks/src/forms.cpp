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


Sphere::Sphere(double r, Color cl, Point org)
{
    radius = r;
    col = cl;
    pos = org;
}

void Sphere::setPos(Point pos){
    this->pos = pos;
}


void Sphere::update(double delta_t)
{
    // Complete this part
}


void Sphere::render()
{
    GLUquadric *quad;
    quad = gluNewQuadric();

    // Complete this part
    gluQuadricDrawStyle(quad, GLU_FILL);


    //Couleur sphère
    glColor3f(this->col.r,this->col.g,this->col.b);
    //Barrycentre
    glTranslated(this->pos.x,this->pos.y,this->pos.z);
    //rotation
    //glRotated(theta, x, , z);

    gluSphere(quad,this->getRadius(), 110, 110);

    gluDeleteQuadric(quad);
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

    // dimension
    //glScaled(0.5,0.5,0.5);

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


Parallepipede_face::Parallepipede_face(Vector v1, Vector v2, Point org, double l, double h, double d, Color cl)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    height = h;
    depth = d;
    col = cl;
}


void Parallepipede_face::update(double delta_t)
{
    // Do nothing, no physics associated to a Cube_face
}


void Parallepipede_face::render()
{
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length*vdir1);
    p3 = p2;
    p3.translate(height*vdir2);
    p4.translate(height*vdir2);


    // dimension
    //glScaled(0.5,0.5,0.5);

    Form::render();
    glBegin(GL_QUADS);
    {
        //extérieur
        //glColor3f(1,1,0);
        glColor3f(col.r, col.g, col.b);
        glVertex3d(p1.x, p1.y, p1.z);
        //glColor3f(0,1,1);
        glColor3f(col.r, col.g, col.b);
        glVertex3d(p2.x, p2.y, p2.z);
        //glColor3f(1,0,1);
        glColor3f(col.r, col.g, col.b);
        glVertex3d(p3.x, p3.y, p3.z);
        //glColor3f(0,1,0);
        glColor3f(col.r, col.g, col.b);
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();
}
