#include <cmath>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include "forms.h"
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <stdint.h>
#include "globals.cpp"
#include <math.h>

//TEST
#define PI 3.14159265

using namespace std;

// Screen dimension constants
const int SCREEN_WIDTH = 1040;
const int SCREEN_HEIGHT = 700;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 20;

// Animation actualization delay (in ms) => 100 updates per second
const int ANIM_DELAY = 10;





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
    //Crée une sphère de manière aléatoire
    //Init random generator
    radius = .5;
    //Color temp(random(),random(),random());
    float ratio = (abs(a)/15.0f);

    if(a>=0){
        //charge positive
        //color setter
        Color tempPos(ratio,ratio,1.0f);
        col=tempPos;
    }
    else{
        Color tempNeg(1.0f,ratio,ratio);
        col=tempNeg;
    }

    //rand()%(borne _maximale - borne_minimale) + borne_minimale;
    pos.x = rand()%(int(longueurFaceExt)-2)+2;
    //std::cout << "longueur = " << longueurFaceExt << "\n10 = " << longueurFaceExt/3 <<std::endl;
    pos.y=0.5;
    pos.z = rand()%(int(largeurFaceExt)-2)+2;
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
    //std::cout<<pos.x<<"\n";
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



Parallepipede_face::Parallepipede_face(Vector v1, Vector v2, Point org, double l, double h, double d, GLuint mtexture)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    height = h;
    depth = d;
    this->texture = mtexture;
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
    //std::cout<< "longueur : " << p2.x << "largeur : " << p2.z;
    p3 = p2;
    p3.translate(height*vdir2);
    p4.translate(height*vdir2);


    // dimension
    //glScaled(0.5,0.5,0.5);
    glBindTexture(GL_TEXTURE_2D, texture);


    Form::render();
    glBegin(GL_QUADS);
    {
        //extérieur
        //glColor3f(1,1,0);
        glTexCoord2d(0,1);

        glVertex3d(p1.x, p1.y, p1.z);
        //glColor3f(0,1,1);
        glTexCoord2d(0,0);
        glVertex3d(p2.x, p2.y, p2.z);
        //glColor3f(1,0,1);
        glTexCoord2d(1,0);
        glVertex3d(p3.x, p3.y, p3.z);
        //glColor3f(0,1,0);
        glTexCoord2d(1,1);
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

//Recupère une position de charge fictive et calcul le vecteur qui passe par la charge courante et la charge fictive.
void Charge::vectDirecteur(Point chargeFictive){
    this->vectPorteur = Vector(getChargePos(), chargeFictive);
}

void Charge::calculCoulomb(Charge *fictive){
    double chargeFictiveValue = fictive->chargeValue;
    vectDirecteur(fictive->sphere.getSpherePos());
    this -> force += ((8.99*pow(10,9))*(this -> chargeValue * fictive->chargeValue)/pow(vectPorteur.norm(),3))*(this -> vectPorteur);
}

Point Charge::getChargePos(){
    return this->sphere.getSpherePos();
}


ContenerCharges::ContenerCharges(int numberOfCharge){
    for(size_t i=0; i<numberOfCharge/4; i++){
        //couloir droit
        double chargeValue = rand()%20-10;
        int intChargeValue = (int)chargeValue;
        tab.push_back(new Charge(chargeValue, Sphere(intChargeValue), Vector(), rand()%2, Vector()));
        Point A((double)(longueurFaceExt/numberOfCharge)*(i*4), 0.5,largeurFaceExt/3);
        tab.at(tab.size()-1)->setPos(A);

        //couloir gauche
        double chargeValue1 = rand()%20-10;
        int intChargeValue1 = (int)chargeValue1;
        tab.push_back(new Charge(chargeValue1, Sphere(intChargeValue1), Vector(), rand()%2, Vector()));
        Point A1((double)(longueurFaceExt/numberOfCharge)*(i*4), 0.5,(largeurFaceExt/3)*2.0);
        tab.at(tab.size()-1)->setPos(A1);
    }

    /*for(size_t i=0; i<numberOfCharge/4; i++){
        //Triangles
        double chargeValue = rand()%20-10;
        int intChargeValue = (int)chargeValue;
        tab.push_back(new Charge(chargeValue, Sphere(intChargeValue), Vector(), rand()%2, Vector()));
        Point A((double)(longueurFaceExt/numberOfCharge)*(i*4), 0.5,largeurFaceExt/3);
        tab.at(tab.size()-1)->setPos(A);
    }*/


    /*this->numberOfCharge = numberOfCharge;
    for(size_t i=0;i<this->numberOfCharge;i++){
        double chargeValue = rand()%20-10;
        int intChargeValue = (int)chargeValue;
        tab.push_back(new Charge(chargeValue, Sphere(intChargeValue), Vector(), rand()%2, Vector()));
    }*/

    ChargeMobile = new Charge();
}

void ContenerCharges::render(){


    glPushMatrix(); // Preserve the camera viewing point for further forms
    // Render the coordinates system
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(5, 0, 0);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 5, 0);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 0, 5);
    }
    glEnd();
    glPopMatrix(); // Restore the camera viewing point for next object

    // Render the list of forms
    unsigned short i = 0;
    while(i != tab.size())
    {
        glPushMatrix(); // Preserve the camera viewing point for further forms
        tab.at(i)->render();
        glPopMatrix(); // Restore the camera viewing point for next object
        i++;
    }
}

void ContenerCharges::update(double delta_t)
{
    for(size_t i = 0; i < tab.size(); i++){
        tab.at(i)->update(delta_t);
    }
    ChargeMobile->calculCoulomb(tab.at(0));
    std::cout<<ChargeMobile->getVect().x<<" is the value\n";

}

/*void calculChargeFictive(std::vector<Charge*> vecCharge){
    //Todo
}*/

void ContenerCharges::ajoutCharge(Charge* charge){
    tab.push_back(charge);
}

std::vector<Charge*> ContenerCharges::getTab(){
    return this->tab;
}
