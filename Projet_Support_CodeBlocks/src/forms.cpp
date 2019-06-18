#include <cmath>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include "forms.h"
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
 #include <stdint.h>


#define PI 3.14159265

using namespace std;

// Screen dimension constants
const int SCREEN_WIDTH = 1040;
const int SCREEN_HEIGHT = 700;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 20;

// Animation actualization delay (in ms) => 100 updates per second
const int ANIM_DELAY = 10;

//Variables de d�finition du plateau
const double hauteurContourPlateau = 2;
const double longueurFaceExt = 20.;
const double largeurFaceExt = 10.;
const double profondeurFace = 0.5;
const Color clBoard(0,0,255);
const Color clBoardBase(255,0,0);




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

float random(){
return (float)(double(rand()) / (double(RAND_MAX) + 1.0));
}

Sphere::Sphere(int a){
    //Cr�e une sph�re de mani�re al�atoire
    //Init random generator
    srand(time(NULL));
    radius = .5;
    Color temp(random(),random(),random());
    col = temp;
    pos.x = rand()%10+3;
    pos.y=rand()%5+3;
    pos.z = rand()%5+2;
    //TODO LES LIMITATIONS
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


    //Couleur sph�re
    glColor3f(this->col.r,this->col.g,this->col.b);
    //Barrycentre
    glTranslated(this->pos.x,this->pos.y,this->pos.z);
    //rotation
    //glRotated(theta, x, , z);

    gluSphere(quad,this->getRadius(), 110, 110);

    gluDeleteQuadric(quad);
}

Point Sphere::getSpherePos(){
    return this->pos;
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
        //ext�rieur
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

Charge::Charge(double _charge, Sphere _sphere, Vector _force, int _bloquage, Vector _directeur)
{
    chargeValue = _charge;
    sphere = _sphere;
    force = _force;
    bloquage = _bloquage;
    vectPorteur = _directeur;
}

//Render de la charge
void Charge::render()
{
    this->sphere.render();
}

void Charge::update(double delta_t)
{
    //Calculs des charges fictives
    //std::cout<<tabCharges[0] -> getChargePos();
    //std::cout<<this-> tab->chargeValue<<"\n";
}

//Recup�re une position de charge fictive et calcul le vecteur qui passe par la charge courante et la charge fictive.
void Charge::vectDirecteur(Point chargeFictive){
    this->vectPorteur = Vector(getChargePos(), chargeFictive);
}

void Charge::calculCoulomb(Charge fictive){
    double chargeFictiveValue = fictive.chargeValue;
    vectDirecteur(fictive.sphere.getSpherePos());
    this -> force = ((8.99*pow(10,9))*(this -> chargeValue * fictive.chargeValue)/pow(vectPorteur.norm(),3))*(this -> vectPorteur);
}

Point Charge::getChargePos(){
    return this->sphere.getSpherePos();
}


ContenerCharges::ContenerCharges(int numberOfCharge){
    this->numberOfCharge = numberOfCharge;
}

void ContenerCharges::render(){
    for(size_t i=0;i<this->numberOfCharge;i++){
        //Charge(double charge = 0.0, Sphere _sphere = Sphere(), Vector _force = Vector(),
        //     int _bloquage = 1, Vector vectPorteur=Vector());
        Charge *temp = new Charge(rand()%2*pow(-1,rand()), Sphere(1), Vector(), rand()%2, Vector());
        tab.push_back(temp);
    }
}

void ContenerCharges::update(double delta_t)
{
}

void calculChargeFictive(std::vector<Charge*> vecCharge){
    //Todo
}

void ContenerCharges::ajoutCharge(Charge* charge){
    tab.push_back(charge);
}

std::vector<Charge*> ContenerCharges::getTab(){
    return this->tab;
}
