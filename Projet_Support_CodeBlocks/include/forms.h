#ifndef FORMS_H_INCLUDED
#define FORMS_H_INCLUDED

#include "geometry.h"
#include "animation.h"


class Color
{
public:
    float r, g, b;
    Color(float rr = 1.0f, float gg = 1.0f, float bb = 1.0f) {r=rr; g=gg; b=bb;}
};

// Constant Colors
const Color RED(1.0f, 0.0f, 0.0f);
const Color BLUE(0.0f, 0.0f, 1.0f);
const Color GREEN(0.0f, 1.0f, 0.0f);
const Color YELLOW(1.0f, 1.0f, 0.0f);
const Color WHITE(1.0f, 1.0f, 1.0f);


// Generic class to render and animate an object
class Form
{
protected:
    Color col;
    Animation anim;
public:
    Animation& getAnim() {return anim;}
    void setAnim(Animation ani) {anim = ani;}
    // This method should update the anim object with the corresponding physical model
    // It has to be done in each inherited class, otherwise all forms will have the same movements !
    // Virtual method for dynamic function call
    // Pure virtual to ensure all objects have their physics implemented
    virtual void update(double delta_t) = 0;
    // Virtual method : Form is a generic type, only setting color and reference position
    virtual void render();
};


// A particular Form
class Sphere : public Form
{
private:
    // The sphere center is aligned with the coordinate system origin
    // => no center required here, information is stored in the anim object
    double radius;
    Point pos;
public:
    Sphere(double r = 1.0, Color cl = Color(), Point p = Point());
    const double getRadius() {return radius;}
    void setRadius(double r) {radius = r;}
    void update(double delta_t);
    void render();
};


// A face of a cube
class Cube_face : public Form
{
private:
    Vector vdir1, vdir2;
    double length, width;
public:
    Cube_face(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,0,1),
          Point org = Point(), double l = 1.0, double w = 1.0,
          Color cl = Color());
    void update(double delta_t);
    void render();
};


// Charges
class Charge: public Form
{
protected:
    //charge associ�e
    double chargeValue;
    //sph�re de repr�sentation
    Sphere sphere;
    //force subi par la charge
    Vector force;
    //Si 1 la charge est fig�e dans l'espace, sinon elle bouge
    bool bloquage;
    //Charge fictive de l'ensemble des autres charges r�unie en une charge.
    //Charge chargeFictive;
public:
    //constructor
    Charge(double charge = 0.0, Sphere _sphere = Sphere(),Vector _force = Vector(0,0,0),
             int _bloquage = 1);
    void render();
    void update(double delta_t);

    //getter chargeValue
    //double getCharge(){return charge;}
    //setter chargeValue
   // void setCharge(double _charge){chargeValue=_charge;}

    //getter force
    //Vector getForce(){return force;}
    //setter force


    //setter charge fictive
    //setter charge force



};

#endif // FORMS_H_INCLUDED
