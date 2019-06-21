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
    float ratio = a/10.0f;
    //std::cout<<"a   : "<<a<<"ratio : "<<ratio<<"\n";

    if(a>=0){
        //charge positive
        //color setter
        Color tempPos(.8f-ratio,.8f-ratio,1.0f);
        col=tempPos;
    }
    else{;
        Color tempNeg(1.f,.8f-ratio,.8f-ratio);
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

Tobogan::Tobogan(Vector v1, Vector v2, Point org, double l, double h, GLuint mtexture)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    height = h;
    this->texture = mtexture;
}


void Tobogan::update(double delta_t)
{
    // Do nothing, no physics associated to a Cube_face
}


void Tobogan::render()
{
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length*vdir1);
    p3 = p2;
    p3.translate(height*vdir2);
    p4.translate(height*vdir2);

    glBindTexture(GL_TEXTURE_2D, texture);


    Form::render();
    glBegin(GL_QUADS);
    {
        glTexCoord2d(0,1);
        glVertex3d(p1.x, p1.y, p1.z);
        //std::cout << "x : " << p1.x << "y : " << p1.y<< "z : " << p1.z;
        glTexCoord2d(0,0);
        glVertex3d(p2.x, p2.y, p2.z);
        glTexCoord2d(1,0);
        glVertex3d(p3.x, p3.y, p3.z);
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
    positionFuture = Point();
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

     //Calculs des charges fictives
    //std::cout<<tabCharges[0] -> getChargePos();
    //std::cout<<this-> tab->chargeValue<<"\n";
    /*ti = 10;
    f=20;
    g = 9.8;
    m = 0.0005489;
    p = m*g;*/

    if(this->bloquage == 0){
        //vector initial



        /*Vector V0 = Vector(1,1,1);
        //Vector X0 = Vector(formlist[i]->getChargePos().x, formlist[i]->getChargePos().y, formlist[i]->getChargePos().z);
        Vector X0 = Vector(-20,20,10);

        Vector a = Vector(0, 0, 0);

        Vector vitesse = a.integral(delta_t) + V0;
        Vector X = vitesse.integral(delta_t) + X0;*/

        //SCharge charge = &formlist[i];
        //this->setPos(Point(X.x+this->getChargePos().x, X.y+this->getChargePos().y, X.z+this->getChargePos().z));
        //std::cout<< "x : " << this->getChargePos().x << "y : " << this->getChargePos().y << "z : " << this->getChargePos().z;

    }

}

//Recupère une position de charge fictive et calcul le vecteur qui passe par la charge courante et la charge fictive.
void Charge::vectDirecteur(Point chargeFictive){
    this->vectPorteur = Vector(chargeFictive,this->getChargePos());
}

void Charge::calculCoulomb(Charge *fictive){
    //double chargeFictiveValue = fictive->chargeValue;
    vectDirecteur(fictive->sphere.getSpherePos());
    this -> force += ((8.99*pow(10,-3))*(this -> chargeValue * fictive->chargeValue)/pow(vectPorteur.norm(),3))*(this -> vectPorteur);
}

Point Charge::getChargePos(){
    return this->sphere.getSpherePos();
}

void Charge::collisionCharge(Charge *charge){
    /*Distance entre 2 points : Norme
    AB=√(xB−xA)²+(yB−yA)²+(zB−zA)².
    */
    //double distance = Vector(this->getChargePos(), charge->getChargePos()).norm();
    double distanceFuture = Vector(this->positionFuture, charge->getChargePos()).norm();
    if(distanceFuture<1.0){
        this->bloquage = 1;

        /*Distance entre 2 points
        AB=√(xB−xA)²+(yB−yA)²+(zB−zA)².
        */
        //Calcul Mouvement final
        /*Vector F = this->getVect();
        Vector A = (pow(10,4))*F;
        Vector V = A.integral(delta_t);
        Vector X = V.integral(delta_t);
        Point Coordonnee(X.x+ChargeMobileCurrentPos.x, 0.5, X.z+ChargeMobileCurrentPos.z);*/
    }
}

bool Charge::collisionMur(double newX, double newZ){
    Point actualCoordinates = this->getChargePos();

    if((this->positionFuture.x <= 0.5 + epaisseurFace || this->positionFuture.x >= longueurFaceExt - epaisseurFace - 0.5)
       && (this->positionFuture.z <= 0.5 + epaisseurFace  || this->positionFuture.z >= largeurFaceExt - epaisseurFace - 0.5)){
        this->setPos(Point(actualCoordinates.x, 0.5, actualCoordinates.z));
        return 1;
    }
    else if(this->positionFuture.x <= 0.5 + epaisseurFace  || this->positionFuture.x >= longueurFaceExt - epaisseurFace - 0.5){
        this->setPos(Point(actualCoordinates.x, 0.5, actualCoordinates.z+newZ));
        return 1;
    }
    else if(this->positionFuture.z <= 0.5 + epaisseurFace  || this->positionFuture.z >= largeurFaceExt - epaisseurFace - 0.5){
        this->setPos(Point(actualCoordinates.x+newX, 0.5, actualCoordinates.z));
        return 1;
    }

    return 0;
}


ContenerCharges::ContenerCharges(int numberOfCharge){
    for(int i=0; i<numberOfCharge/4-1; i++){
        //couloir droit
        double randomNumber = (rand()%10)*1.0;
        double chargeValue = randomNumber/10.0;
        int intChargeValue = (int)randomNumber;
        tab.push_back(new Charge(chargeValue, Sphere(intChargeValue), Vector(), rand()%2, Vector()));
        Point A((double)(longueurFaceExt/numberOfCharge)*(i*4)+8, 0.5,largeurFaceExt/3);
        tab.at(tab.size()-1)->setPos(A);

        //couloir gauche
        double random2 = (rand()%10)*1.0;
        double chargeValue1 = random2/10.0;
        int intChargeValue1 = (int)random2;
        tab.push_back(new Charge(chargeValue1, Sphere(intChargeValue1), Vector(), rand()%2, Vector()));
        Point A1((double)(longueurFaceExt/numberOfCharge)*(i*4)+8, 0.5,(largeurFaceExt/3)*2.0);
        tab.at(tab.size()-1)->setPos(A1);
    }

    /*double chargeValue = -5; //rand()%20-10;
    int intChargeValue = (int)chargeValue;
    tab.push_back(new Charge(chargeValue, Sphere(intChargeValue), Vector(), rand()%2, Vector()));
    tab.at(tab.size()-1)->setColor(RED);
    Point A((double)(longueurFaceExt/3-3), (double)0.5,(double)(largeurFaceExt/3-3));
    tab.at(tab.size()-1)->setPos(A);*/

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


    //ChargeMobile = new Charge(-8, Sphere(.5, RED, Point((double)-15,(double)15,largeurFaceExt/2)), Vector(Point(-largeurFaceExt,largeurFaceExt,largeurFaceExt/2), Point(largeurFaceExt,largeurFaceExt,largeurFaceExt/2)), 0, Vector(0,0,0));
    ChargeMobile = new Charge(-5, Sphere(.5, RED, Point(-4,20,largeurFaceExt/2)), Vector(0,0,0), 0, Vector(0,0,0));
    calculCharge = 0;
    this->numberOfCharge = numberOfCharge;
    this->pause=false;
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
    glPushMatrix(); // Preserve the camera viewing point for further forms
    ChargeMobile->render();
    glPopMatrix(); // Restore the camera viewing point for next object
}

void ContenerCharges::update(double delta_t)
{

    for(size_t i = 0; i < this->tab.size() && !this->isPause(); i++){
        this->ChargeMobile->collisionCharge(tab.at(i));
    }

    //std::cout<<"est bloque : "<<this->ChargeMobile->estBloquee()<<"\n";

    if(!this->ChargeMobile->estBloquee() && this->isPause()==false){
        if(this->ChargeMobile->getChargePos().y > 0.5)
        {
            Vector G = Vector(0,-9.81,0);
            Vector VInit = Vector(4,-10,0.5);
            Vector V = G.integral(delta_t)+VInit;
            //Delta position
            //Vector XInit = Vector(-4,20,largeurFaceExt/2);
            Vector X = V.integral(delta_t);

            this->ChargeMobile->setPos(Point(X.x+this->ChargeMobile->getChargePos().x,X.y+this->ChargeMobile->getChargePos().y,X.z+this->ChargeMobile->getChargePos().z));
        }
        else{
            //this->ChargeMobile->setPos(Point(this->ChargeMobile->getChargePos().x,0.5,this->ChargeMobile->getChargePos().z));
            /*if(this->calculCharge==0 || calculCharge >= this->numberOfCharge)
            {
                this->ChargeMobile->initVectForce();
                this->calculCharge = 0;
            }
            if(this->calculCharge<this->numberOfCharge)
            {*/
            for(size_t y=0; y<tab.size();y++){
                ChargeMobile->calculCoulomb(tab.at(y));
            }


            //Calcul Mouvement
            Vector F = ChargeMobile->getVect();
            Vector A = (pow(10,4))*F;
            Vector V = A.integral(delta_t);
            Vector X = V.integral(delta_t);

            //std::cout << F.x << " " << F.y << " " << F.z << std::endl;
            //std::cout << A.x << " " << A.y << " " << A.z << std::endl;
            //std::cout << V.x << " " << V.y << " " << V.z << std::endl;
            //Point ChargeMobile = ChargeMobile->getChargePos();
            Point ChargeMobileCurrentPos=ChargeMobile->getChargePos();
            Point Coordonnee(X.x+ChargeMobileCurrentPos.x, 0.5, X.z+ChargeMobileCurrentPos.z);
            //Point Coordonnee(X.x,0.5,X.z);
            //std::cout << X.x << " " << X.y << " " << X.z << std::endl;

            //sortie de plan
            //pour X
            /*if(Coordonnee.x<1){Coordonnee.x=0.5+epaisseurFace;}
            if(Coordonnee.x>longueurFaceExt-0.5){Coordonnee.x=longueurFaceExt-0.5-epaisseurFace;}
            //pour Z
            if(Coordonnee.z<1){Coordonnee.z=0.5+epaisseurFace;}
            if(Coordonnee.z>largeurFaceExt-0.5){Coordonnee.z=largeurFaceExt-0.5-epaisseurFace;}
            //Pour Y
            //NADA
            */

            //Affectation de la position future à l'attribu futurPos de la chargeMobile
            this->ChargeMobile->setPositionFuture(Coordonnee);
            for(size_t i = 0; i < this->tab.size(); i++){
                this->ChargeMobile->collisionCharge(tab.at(i));
            }

            //UpdateCollision à faire en dehors de l'updateCharge et Recalculer les forces
            if(!this->ChargeMobile->estBloquee()){
                if(!this->ChargeMobile->collisionMur(X.x,X.z)){
                    this->ChargeMobile->setPos(Coordonnee);
                }
            }
            //std::cout<<"Coordonnee charge Mobile : "<<ChargeMobile->getChargePos().x<<"   "<<ChargeMobile->getChargePos().y<<"   "<<ChargeMobile->getChargePos().z<<" is the value\n";


            //Affcihage vector force


            //calculCharge++;
            for(size_t y=0; y<tab.size();y++){
                tab.at(y)->update(delta_t);
            }




            //ChargeMobile->calculCoulomb(tab.at(0));
            //std::cout<<ChargeMobile->getVect().x<<" is the value\n";
            //glPushMatrix(); // Preserve the camera viewing point for further forms

            /*Mouvement de la charge
            Point posChargeMobile(ChargeMobile->getVect().x+ChargeMobile->getChargePos().x,ChargeMobile->getVect().y+ChargeMobile->getChargePos().y,ChargeMobile->getVect().z+ChargeMobile->getChargePos().z);
            std::cout<<"Charge Mobile : "<<ChargeMobile->getVect().x<<"  "<<ChargeMobile->getVect().y<<"\n";
            ChargeMobile->setPos(posChargeMobile);
            glPopMatrix();*/

            /*}*/
        }

    }


    /*for(size_t i = 0; i < tab.size(); i++)
    {
        tab.at(i)->update(delta_t);
    }
    */



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

void ContenerCharges::moveRight(){
    Point futur(this->ChargeMobile->getChargePos().x+1,0.5,this->ChargeMobile->getChargePos().z);
    if(this->isPause() && isSuperposed(futur)){
            this->ChargeMobile->setPos(futur);
            this->ChargeMobile->setPositionFuture(futur);
    }
}

void ContenerCharges::moveLeft(){
    Point futur(this->ChargeMobile->getChargePos().x-1,0.5,this->ChargeMobile->getChargePos().z);
    if(this->isPause() && isSuperposed(futur)){
            this->ChargeMobile->setPos(futur);
            this->ChargeMobile->setPositionFuture(futur);
    }
}

void ContenerCharges::moveUp(){
    Point futur(this->ChargeMobile->getChargePos().x,0.5,this->ChargeMobile->getChargePos().z-1);
    if(this->isPause() && isSuperposed(futur)){
            this->ChargeMobile->setPos(futur);
            this->ChargeMobile->setPositionFuture(futur);
    }
}

void ContenerCharges::moveDown(){
    Point futur(this->ChargeMobile->getChargePos().x,0.5,this->ChargeMobile->getChargePos().z+1);
    if(this->isPause() && isSuperposed(futur)){
        this->ChargeMobile->setPos(futur);
        this->ChargeMobile->setPositionFuture(futur);
    }
}

bool ContenerCharges::isSuperposed(Point futur){
    for(size_t i=0; i<this->tab.size(); i++){
        double distance = Vector(futur, tab.at(i)->getChargePos()).norm();
        //Si la distance anticipée pose un problème de superposition on retourn faux
        if(distance<1.0) return false;
    }
    //conditions murs
    if(futur.z>(largeurFaceExt-epaisseurFace) || futur.z<epaisseurFace) return false;
    if(futur.x>longueurFaceExt-epaisseurFace || futur.x<epaisseurFace) return false;
    return true;
}
