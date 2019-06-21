
#ifndef FORMS_H_INCLUDED
#define FORMS_H_INCLUDED

#include "geometry.h"
#include "animation.h"
#include <vector>
#include "sdlglutils.h"


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
    Sphere(double r = 1.0, Color cl = Color(), Point org = Point());
    Sphere(int a);
    const double getRadius() {return radius;}
    void setRadius(double r) {radius = r;}
    void update(double delta_t);
    void render();
    void setPos(Point pos);
    Point getSpherePos();

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

// A face of parallepipede
class Parallepipede_face : public Form
{
private :
    Vector vdir1, vdir2;
    double length, height, depth;
    GLuint texture;
public:
    Parallepipede_face(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,0,1),
          Point org = Point(), double length = 1.0, double height = 1.0, double depth = 0.0,
          GLuint texture = loadTexture("./textures/02.jpg"));
    void update(double delta_t);
    void render();
};

class Tobogan : public Form
{
    private :
    Vector vdir1, vdir2;
    double length, height, depth;
    GLuint texture;
public:
    Tobogan(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,0,1),
          Point org = Point(), double length = 1.0, double height = 1.0,
          GLuint texture = loadTexture("./textures/02.jpg"));
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
    //force subi par la charge en double les vecteurs unitaires seront calculés à la main
    Vector force;
    //Si 1 la charge est fige dans l'espace, sinon elle bouge
    bool bloquage;
    //Charge fictive de l'ensemble des autres charges r�unie en une charge.
    //Charge chargeFictive;
    Vector vectPorteur;
    Point positionFuture;

public:
    //constructor
    Charge(double charge = 0.0, Sphere _sphere = Sphere(), Vector _force = Vector(),
             int _bloquage = 1, Vector vectPorteur=Vector());
    void render();
    void update(double delta_t);
    void vectDirecteur(Point chargeFictive);
    void calculCoulomb(Charge* fictive);
    Point getChargePos();
    Vector getVect(){return this->force;}
    void initVectForce(){force=Vector();}
    void setPos(Point p){this->sphere.setPos(p);}
    void setForce(Vector F){this->force=F;}
    bool estBloquee(){ return this->bloquage;}
    void collisionCharge(Charge *charge);
    bool collisionMur(Point actualCoordinates, double newX, double newZ);
    void setColor(Color c){this->col = c;}
    void setPositionFuture(Point P){positionFuture=P;}
    //void calculChargeFictive(std::vector<Charge*> vecCharge);
    /*std::vector<Charge*> vecCharge;
    void actualiseVec(std::vector<Charge*> vecCharge);*/
};

class ContenerCharges: public Form{
private:
    std::vector<Charge*> tab;
    int numberOfCharge;
    Charge *ChargeMobile;
    int calculCharge;
public:
    ContenerCharges(int numberOfCharge=0);
    void render();
    void update(double delta_t);
    void ajoutCharge(Charge* charge);
    Vector updatePosition(double delta_t, bool ret);
    std::vector<Charge*> getTab();

};



#endif // FORMS_H_INCLUDED
