// Using SDL, SDL OpenGL and standard IO
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>

// Module for space geometry
#include "geometry.h"
// Module for generating and rendering forms
#include "forms.h"


using namespace std;


/***************************************************************************/
/* Constants and functions declarations                                    */
/***************************************************************************/
// Screen dimension constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 20;

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
    gluLookAt(cam_pos.x,cam_pos.y,cam_pos.z,   cam_dir.x,cam_dir.y,cam_dir.z,    cam_h.x,cam_h.y,cam_h.z);
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

    //Déplacer le centre de gravité du cube : barrycentre
    //glTranslated(0,2,4);

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
        Point camera_position(0, 5, -15);
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

        //Variables de définition du plateau
        double hauteurContourPlateau = 2;
        double longueurFaceExt = 20.;
        double largeurFaceExt = 10.;
        double profondeurFace = 0.5;
        Color clBoard(0,0,255);
        Color clBoardBase(255,0,0);
        //FACE 1 EXTERIEUR PLATEAU
        Parallepipede_face *firstCloseExtBoard = NULL;
        firstCloseExtBoard = new Parallepipede_face(Vector(1,0,0), Vector(0,1,0), Point(0, 0, 0), longueurFaceExt, hauteurContourPlateau, profondeurFace, clBoard);
        forms_list[number_of_forms] = firstCloseExtBoard;
        number_of_forms++;
        //FACE 2 EXTERIEUR PLATEAU
        Parallepipede_face *secondCloseExtBoard = NULL;
        secondCloseExtBoard = new Parallepipede_face(Vector(0,0,1), Vector(0,1,0), Point(0, 0, 0), largeurFaceExt, hauteurContourPlateau, profondeurFace, clBoard);
        forms_list[number_of_forms] = secondCloseExtBoard;
        number_of_forms++;
        //FACE 3 EXTERIEUR PLATEAU
        Parallepipede_face *thirdCloseExtBoard = NULL;
        thirdCloseExtBoard = new Parallepipede_face(Vector(0,0,1), Vector(0,1,0), Point(longueurFaceExt, 0, 0), largeurFaceExt, hauteurContourPlateau, profondeurFace, clBoard);
        forms_list[number_of_forms] = thirdCloseExtBoard;
        number_of_forms++;
        //FACE 4 EXTERIEUR PLATEAU
        Parallepipede_face *fourthCloseExtBoard = NULL;
        fourthCloseExtBoard = new Parallepipede_face(Vector(1,0,0), Vector(0,1,0), Point(0, 0, largeurFaceExt), longueurFaceExt, hauteurContourPlateau, profondeurFace, clBoard);
        forms_list[number_of_forms] = fourthCloseExtBoard;
        number_of_forms++;

        //profondeur
        Parallepipede_face *depthF1 = NULL;
        depthF1 = new Parallepipede_face(Vector(1,0,0), Vector(0,1,0), Point(0, 0, profondeurFace), longueurFaceExt, hauteurContourPlateau, profondeurFace, clBoard);
        forms_list[number_of_forms] = depthF1;
        number_of_forms++;
        Parallepipede_face *depthF2 = NULL;
        depthF2 = new Parallepipede_face(Vector(0,0,1), Vector(0,1,0), Point(profondeurFace, 0, 0), largeurFaceExt, hauteurContourPlateau, profondeurFace, clBoard);
        forms_list[number_of_forms] = depthF2;
        number_of_forms++;
        Parallepipede_face *depthF3 = NULL;
        depthF3 = new Parallepipede_face(Vector(0,0,1), Vector(0,1,0), Point(longueurFaceExt-profondeurFace, 0, 0), largeurFaceExt, hauteurContourPlateau, profondeurFace, clBoard);
        forms_list[number_of_forms] = depthF3;
        number_of_forms++;
        Parallepipede_face *depthF4 = NULL;
        depthF4 = new Parallepipede_face(Vector(1,0,0), Vector(0,1,0), Point(0, 0, largeurFaceExt-profondeurFace), longueurFaceExt, hauteurContourPlateau, profondeurFace, clBoard);
        forms_list[number_of_forms] = depthF4;
        number_of_forms++;

        //DESSUS DES MURS DE LA PROFONDEUR
        Parallepipede_face *roofF1 = NULL;
        depthF1 = new Parallepipede_face(Vector(1,0,0), Vector(0,0,1), Point(0, hauteurContourPlateau, 0), longueurFaceExt, profondeurFace, profondeurFace, clBoard);
        forms_list[number_of_forms] = depthF1;
        number_of_forms++;
        Parallepipede_face *roofF2 = NULL;
        roofF2 = new Parallepipede_face(Vector(0,0,1), Vector(1,0,0), Point(0, hauteurContourPlateau, 0), largeurFaceExt, profondeurFace, profondeurFace, clBoard);
        forms_list[number_of_forms] = roofF2;
        number_of_forms++;
        Parallepipede_face *roofF3 = NULL;
        roofF3 = new Parallepipede_face(Vector(0,0,1), Vector(1,0,0), Point(longueurFaceExt-profondeurFace, hauteurContourPlateau, 0), largeurFaceExt, profondeurFace, profondeurFace, clBoard);
        forms_list[number_of_forms] = roofF3;
        number_of_forms++;
        Parallepipede_face *roofF4 = NULL;
        roofF4 = new Parallepipede_face(Vector(1,0,0), Vector(0,0,1), Point(0, hauteurContourPlateau, largeurFaceExt-profondeurFace), longueurFaceExt, profondeurFace, profondeurFace, clBoard);
        forms_list[number_of_forms] = roofF4;
        number_of_forms++;

        //PLATEAU
        Parallepipede_face *boardBase = NULL;
        boardBase = new Parallepipede_face(Vector(1,0,0), Vector(0,0,1), Point(0, 0, 0), longueurFaceExt, largeurFaceExt, profondeurFace, clBoardBase);
        forms_list[number_of_forms] = boardBase;
        number_of_forms++;


        //Gère la rotation des sphères
        double theta=0.0;
        //Satellite1
        Color clrSt1(0,218,0);
        Sphere *satellite1 = NULL;
        satellite1 = new Sphere(0.5, clrSt1, Point(2, 0, 0));
        forms_list[number_of_forms] = satellite1;
        number_of_forms++;

        //Satellite2
        Color clrSt2(2, 218, 0);
        Sphere *satellite2 = NULL;
        satellite2 = new Sphere(0.5, clrSt2, Point(0, 2, 0));
        forms_list[number_of_forms] = satellite2;
        number_of_forms++;

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
                    case SDLK_w:
                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    //Modif Position camera
                    case SDLK_a :
                        camera_position.x = camera_position.x + 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_q :
                        camera_position.x = camera_position.x - 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_z :
                        camera_position.z = camera_position.z + 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_s :
                        camera_position.z = camera_position.z - 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_e :
                        camera_position.y = camera_position.y + 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_d :
                        camera_position.y = camera_position.y - 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;

                    //Modif Direction camera
                    case SDLK_r :
                        camera_rotation.x = camera_rotation.x + 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_f :
                        camera_rotation.x = camera_rotation.x - 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_t :
                        camera_rotation.z = camera_rotation.z + 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_g :
                        camera_rotation.z = camera_rotation.z - 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_y :
                        camera_rotation.y = camera_rotation.y + 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_h :
                        camera_rotation.y = camera_rotation.y - 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;

                    //Modif Hauteur camera
                    case SDLK_u :
                        camera_hauteur.x = camera_hauteur.x + 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_j :
                        camera_hauteur.x = camera_hauteur.x - 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_i :
                        camera_hauteur.z = camera_hauteur.z + 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_k :
                        camera_hauteur.z = camera_hauteur.z - 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_o :
                        camera_hauteur.y = camera_hauteur.y + 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
                        break;
                    case SDLK_l :
                        camera_hauteur.y = camera_hauteur.y - 1;
                        cout << "position : "  << camera_position.x << ", " << camera_position.y << ", " << camera_position.z << endl;
                        cout << "rotation : "  << camera_rotation.x << ", " << camera_rotation.y << ", " << camera_rotation.z << endl;
                        cout << "rotation : "  << camera_hauteur.x << ", " << camera_hauteur.y << ", " << camera_hauteur.z << endl;
                        cout << endl << "______________-----NEXT-----______________"  << endl;
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
            theta+=0.05;
            //std::cout << "theta : " << theta;
            satellite1->setPos(Point(2, cos(theta), sin(theta))) ;
            satellite2->setPos(Point(cos(theta), 2, sin(theta))) ;


            // Render the scene
            render(forms_list, camera_position, camera_rotation, camera_hauteur);

            // Update window screen
            SDL_GL_SwapWindow(gWindow);
        }
    }

    // Free resources and close SDL
    close(&gWindow);

    return 0;
}
