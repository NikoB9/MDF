// Using SDL, SDL OpenGL and standard IO
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include <math.h>
#include <vector>
#include "sdlglutils.h"
#include "globals.cpp"

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

//#include "sdlglutils.h"


// Module for space geometry
#include "geometry.h"
// Module for generating and rendering forms
#include "forms.h"
//Include Charge
#include "projet.h"

#define PI 3.14159265
#define NOMBRE_CHARGE 1

using namespace std;


/***************************************************************************/
/* Constants and functions declarations                                    */
/***************************************************************************/
// Screen dimension constants
const int SCREEN_WIDTH = 1040;
const int SCREEN_HEIGHT = 700;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 40+NOMBRE_CHARGE;

// Animation actualization delay (in ms) => 100 updates per second
const Uint32 ANIM_DELAY = 10;




// Starts up SDL, creates window, and initializes OpenGL
bool init(SDL_Window** window, SDL_GLContext* context);

// Initializes matrices and clear color
bool initGL();

// Updating forms for animation
void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t);

// Renders scene to the screen
const void render(Form* formlist[MAX_FORMS_NUMBER], const Point &cam_pos, const Point &cam_dir, const Point &cam_h);

// Frees media and shuts down SDL
void close(SDL_Window** window);


/***************************************************************************/
/* Functions implementations                                               */
/***************************************************************************/
bool init(SDL_Window** window, SDL_GLContext* context)
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        // Use OpenGL 2.1
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        // Create window
        *window = SDL_CreateWindow( "TP intro OpenGL / SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( *window == NULL )
        {
            cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            // Create context
            *context = SDL_GL_CreateContext(*window);
            if( *context == NULL )
            {
                cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << endl;
                success = false;
            }
            else
            {
                // Use Vsync
                if(SDL_GL_SetSwapInterval(1) < 0)
                {
                    cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << endl;
                }

                // Initialize OpenGL
                if( !initGL() )
                {
                    cout << "Unable to initialize OpenGL!"  << endl;
                    success = false;
                }
            }
        }
    }

    return success;
}


bool initGL()
{
    bool success = true;
    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport : use all the window to display the rendered scene
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Fix aspect ratio and depth clipping planes
    gluPerspective(40.0, (GLdouble)SCREEN_WIDTH/SCREEN_HEIGHT, 1.0, 100.0);


    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize clear color : black with no transparency
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Activate Z-Buffer


    // Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        cout << "Error initializing OpenGL!  " << gluErrorString( error ) << endl;
        success = false;
    }

    glEnable(GL_DEPTH_TEST);

    //ACTIVATION DE LA TEXTURE
    glEnable(GL_TEXTURE_2D);

    return success;
}

void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t)
{
    // Update the list of forms
    unsigned short i = 0;
    while(formlist[i] != NULL)
    {
        formlist[i]->update(delta_t);
        i++;
    }
}

const void render(Form* formlist[MAX_FORMS_NUMBER], const Point &cam_pos, const Point &cam_dir, const Point &cam_h)
{
    // Clear color buffer and Z-Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Set the camera position and parameters
    //glTranslated(-longueurFaceExt/2,0,-largeurFaceExt/2);
    gluLookAt(cam_pos.x,cam_pos.y,cam_pos.z,   cam_dir.x,cam_dir.y,cam_dir.z,    cam_h.x,cam_h.y,cam_h.z);
    //glTranslated(longueurFaceExt/2,0,largeurFaceExt/2);
    // Isometric view
    glRotated(-45, 0, 1, 0);
    glRotated(30, 1, 0, -1);

    // X, Y and Z axis
    glScaled(0.5,0.5,0.5);

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

    //D�placer le centre de gravit� du cube : barrycentre
    glTranslated(-longueurFaceExt/2,0,-largeurFaceExt/2);

    // Render the list of forms
    unsigned short i = 0;
    while(formlist[i] != NULL)
    {
        glPushMatrix(); // Preserve the camera viewing point for further forms
        formlist[i]->render();
        glPopMatrix(); // Restore the camera viewing point for next object
        i++;
    }
}

void close(SDL_Window** window)
{
    //Destroy window
    SDL_DestroyWindow(*window);
    *window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}


/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char* args[])
{
    // The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    // OpenGL context
    SDL_GLContext gContext;

    //al�atoire active
    srand(time(NULL));


    // Start up SDL and create window
    if( !init(&gWindow, &gContext))
    {
        cout << "Failed to initialize!" << endl;
    }
    else
    {
        // Main loop flag
        bool quit = false;
        Uint32 current_time, previous_time, elapsed_time;

        // Event handler
        SDL_Event event;

        // Camera position
        Point camera_position(1, 5, -10);
        double theta=1.61087, phi=1.5061, rho=1.;
        if(longueurFaceExt > largeurFaceExt){
            rho = rho + longueurFaceExt/2;
        }
        else{
            rho = rho + largeurFaceExt/2;
        }
        // Rotation camera
        Point camera_rotation(0, 0, 0);
        // hauteur camera
        Point camera_hauteur(0, 1, 0);

        // The forms to render
        Form* forms_list[MAX_FORMS_NUMBER];
        unsigned short number_of_forms = 0, i;
        for (i=0; i<MAX_FORMS_NUMBER; i++)
        {
            forms_list[i] = NULL;
        }
        // Create here specific forms and add them to the list...
        // Don't forget to update the actual number_of_forms !


        //TEXTURES
        GLuint texture_sol = loadTexture("./textures/02.jpg");
        GLuint texture_mur = loadTexture("./textures/vecteur7.jpg");


        //FACE 1 EXTERIEUR PLATEAU
        Parallepipede_face *firstCloseExtBoard = NULL;
        firstCloseExtBoard = new Parallepipede_face(Vector(1,0,0), Vector(0,1,0), Point(0, 0, 0), longueurFaceExt, hauteurContourPlateau, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = firstCloseExtBoard;
        number_of_forms++;
        //FACE 2 EXTERIEUR PLATEAU
        Parallepipede_face *secondCloseExtBoard = NULL;
        secondCloseExtBoard = new Parallepipede_face(Vector(0,0,1), Vector(0,1,0), Point(0, 0, 0), largeurFaceExt, hauteurContourPlateau, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = secondCloseExtBoard;
        number_of_forms++;
        //FACE 3 EXTERIEUR PLATEAU
        Parallepipede_face *thirdCloseExtBoard = NULL;
        thirdCloseExtBoard = new Parallepipede_face(Vector(0,0,1), Vector(0,1,0), Point(longueurFaceExt, 0, 0), largeurFaceExt, hauteurContourPlateau, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = thirdCloseExtBoard;
        number_of_forms++;
        //FACE 4 EXTERIEUR PLATEAU
        Parallepipede_face *fourthCloseExtBoard = NULL;
        fourthCloseExtBoard = new Parallepipede_face(Vector(1,0,0), Vector(0,1,0), Point(0, 0, largeurFaceExt), longueurFaceExt, hauteurContourPlateau, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = fourthCloseExtBoard;
        number_of_forms++;

        //profondeur
        Parallepipede_face *depthF1 = NULL;
        depthF1 = new Parallepipede_face(Vector(1,0,0), Vector(0,1,0), Point(0, 0, epaisseurFace), longueurFaceExt, hauteurContourPlateau, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = depthF1;
        number_of_forms++;
        Parallepipede_face *depthF2 = NULL;
        depthF2 = new Parallepipede_face(Vector(0,0,1), Vector(0,1,0), Point(epaisseurFace, 0, 0), largeurFaceExt, hauteurContourPlateau, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = depthF2;
        number_of_forms++;
        Parallepipede_face *depthF3 = NULL;
        depthF3 = new Parallepipede_face(Vector(0,0,1), Vector(0,1,0), Point(longueurFaceExt-epaisseurFace, 0, 0), largeurFaceExt, hauteurContourPlateau, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = depthF3;
        number_of_forms++;
        Parallepipede_face *depthF4 = NULL;
        depthF4 = new Parallepipede_face(Vector(1,0,0), Vector(0,1,0), Point(0, 0, largeurFaceExt-epaisseurFace), longueurFaceExt, hauteurContourPlateau, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = depthF4;
        number_of_forms++;

        //DESSUS DES MURS DE LA PROFONDEUR
        Parallepipede_face *roofF1 = NULL;
        roofF1 = new Parallepipede_face(Vector(1,0,0), Vector(0,0,1), Point(0, hauteurContourPlateau, 0), longueurFaceExt, epaisseurFace, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = roofF1;
        number_of_forms++;
        Parallepipede_face *roofF2 = NULL;
        roofF2 = new Parallepipede_face(Vector(0,0,1), Vector(1,0,0), Point(0, hauteurContourPlateau, 0), largeurFaceExt, epaisseurFace, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = roofF2;
        number_of_forms++;
        Parallepipede_face *roofF3 = NULL;
        roofF3 = new Parallepipede_face(Vector(0,0,1), Vector(1,0,0), Point(longueurFaceExt-epaisseurFace, hauteurContourPlateau, 0), largeurFaceExt, epaisseurFace, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = roofF3;
        number_of_forms++;
        Parallepipede_face *roofF4 = NULL;
        roofF4 = new Parallepipede_face(Vector(1,0,0), Vector(0,0,1), Point(0, hauteurContourPlateau, largeurFaceExt-epaisseurFace), longueurFaceExt, epaisseurFace, epaisseurFace, texture_mur);
        forms_list[number_of_forms] = roofF4;
        number_of_forms++;

        //PLATEAU
        Parallepipede_face *boardBase = NULL;
        boardBase = new Parallepipede_face(Vector(1,0,0), Vector(0,0,1), Point(0, 0, 0), longueurFaceExt, largeurFaceExt, epaisseurFace, texture_sol);
        forms_list[number_of_forms] = boardBase;
        number_of_forms++;

        //Pente pour faire glisser la boule
        /*Tobogan *pente = NULL;
        pente = new Tobogan(Vector(-1,1,0), Vector(0,0,1), Point((epaisseurFace+hauteurContourPlateau), 0, (largeurFaceExt/2)-1.5 ), largeurFaceExt, 3, texture_mur);
        forms_list[number_of_forms] = pente;
        number_of_forms++;*/


        //Cr�ation et positionner les charges
        ContenerCharges *contener=NULL;
        contener = new ContenerCharges(NOMBRE_CHARGE);
        forms_list[number_of_forms]=contener;
        number_of_forms++;

        /*Charge *pCharge1, *pCharge2;
        Color col(RED), col1(BLUE);
        Point po(longueurFaceExt/2,0.5,largeurFaceExt/2);
        Point po1(longueurFaceExt/2-3,0.5,largeurFaceExt/2-3);

        pCharge1 = new Charge(3, Sphere(0.5, col, po), Vector(),1,Vector());
        pCharge2 = new Charge(3, Sphere(0.5, col1, po1), Vector(), 1, Vector());
        forms_list[number_of_forms]=pCharge1;
        number_of_forms++;
        forms_list[number_of_forms]=pCharge2;
        number_of_forms++;

        pCharge1->initVectForce();
        pCharge2->initVectForce();

        pCharge1->calculCoulomb(pCharge2);

        std::cout<<"Vecteur : "<<pCharge1->getVect().x<<"  "<<pCharge1->getVect().y<<"  "<<pCharge1->getVect().z<<"  \n";
        for(int z=0;z<100;z++){
            pCharge1->initVectForce();
            pCharge1->calculCoulomb(pCharge2);
            std::cout<<"Vecteur : "<<pCharge1->getVect().x<<"  "<<pCharge1->getVect().y<<"  "<<pCharge1->getVect().z<<"  \n";
            //pCharge1->setPos(pCharge1->getChargePos().x+pCharge1(->))
        }*/
        /*Charge *pCharge = NULL;
        pCharge = new*/

        // Get first "current time"
        previous_time = SDL_GetTicks();
        // While application is running
        while(!quit)
        {
            // Handle events on queue
            while(SDL_PollEvent(&event) != 0)
            {
                int x = 0, y = 0;
                SDL_Keycode key_pressed = event.key.keysym.sym;

                switch(event.type)
                {
                // User requests quit
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    // Handle key pressed with current mouse position
                    SDL_GetMouseState( &x, &y );

                    switch(key_pressed)
                    {
                    // Quit the program when 'q' or Escape keys are pressed
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    //Modif Position camera
                     case SDLK_UP:
                        phi-=1.5* PI / 180.0;
                        std::cout<<"rho : "<<rho<<"  theta : "<<theta<<"   phi  : "<<phi<<" \n";
                        std::cout<<"x : "<<camera_position.x<<"  y:"<<camera_position.y<<"  z:"<<camera_position.z<<"\n";
                        break;
                    case SDLK_DOWN:
                        phi+=1.5* PI / 180.0;
                        std::cout<<"rho : "<<rho<<"  theta : "<<theta<<"   phi  : "<<phi<<" \n";
                        std::cout<<"x : "<<camera_position.x<<"  y:"<<camera_position.y<<"  z:"<<camera_position.z<<"\n";
                        break;

                    case SDLK_RIGHT:
                        theta-=1.5* PI / 180.0;
                        std::cout<<"rho : "<<rho<<"  theta : "<<theta<<"   phi  : "<<phi<<" \n";
                        std::cout<<"x : "<<camera_position.x<<"  y:"<<camera_position.y<<"  z:"<<camera_position.z<<"\n";
                        break;
                     case SDLK_LEFT:
                        theta+=1.5* PI / 180.0;
                        std::cout<<"rho : "<<rho<<"  theta : "<<theta<<"   phi  : "<<phi<<" \n";
                        std::cout<<"x : "<<camera_position.x<<"  y:"<<camera_position.y<<"  z:"<<camera_position.z<<"\n";
                        break;

                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            }

            // Update the scene
            current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
            elapsed_time = current_time - previous_time;
            if (elapsed_time > ANIM_DELAY)
            {
                previous_time = current_time;
                update(forms_list, 1e-3 * elapsed_time); // International system units : seconds
            }

            //modif positoin du disque
            //thetaSpheres+=0.05;
            //std::cout << "theta : " << theta;
            //satellite1->setPos(Point(2, cos(thetaSpheres), sin(thetaSpheres))) ;
            //satellite2->setPos(Point(cos(thetaSpheres), 2, sin(thetaSpheres))) ;

            //Update Camera
            //x = rho*sin(phi)*cos(theta)   y=rho*sin(phi)*sin(theta)   z=rho*cos(theta)
            camera_position.x = rho*sin(phi)*cos(theta);
            camera_position.z = rho*sin(phi)*sin(theta);
            camera_position.y = rho*cos(phi);

            // Render the scene
            render(forms_list, camera_position, camera_rotation, camera_hauteur);

            // Update window screen
            SDL_GL_SwapWindow(gWindow);
            //
        }
    }

    // Free resources and close SDL
    close(&gWindow);

    return 0;
}
