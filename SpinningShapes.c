#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>
#include <SDL/SDL.h>

const int scr_w   = 1024;
const int scr_h   = 768;
float     rot_pyr = 0.0f;
float     rot_hex = 0.0f;

void hexagon(float x, float y)
{
    float zero = 0.0f;

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-x/2,y,zero);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x/2,y,zero);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x,zero,zero);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(x/2,-y,zero);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-x/2,-y,zero);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-x,zero,zero);
    glEnd();
}

void pyramid(float x, float y, float z)
{
    float zero = 0.0f;

    glBegin(GL_TRIANGLES);         
    glColor3f(1.0f, 0.0f, 0.0f);               
    glVertex3f( zero, y, zero);               
    glVertex3f( x,-y, z);               
    glVertex3f(-x,-y, z);                
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f( zero, y, zero);
    glVertex3f( x,-y,-z);
    glVertex3f( x,-y, z);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f( zero, y, zero);
    glVertex3f(-x,-y, z);
    glVertex3f(-x,-y,-z);
    glColor3f(0.5f, 1.0f, 1.0f);
    glVertex3f( zero, y, zero);
    glVertex3f(-x,-y,-z);
    glVertex3f( x,-y,-z);
    glEnd();
}


void InitGL(int Width, int Height)              // We call this right after our OpenGL window is created.
{
    glViewport(0, 0, Width, Height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);         // This Will Clear The Background Color To Black
    glClearDepth(1.0);                            // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);                         // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);                      // Enables Depth Testing
    glShadeModel(GL_SMOOTH);                      // Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                             // Reset The Projection Matrix

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);     // Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);

}

void DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);           // Clear The Screen And The Depth Buffer
    glLoadIdentity();                             // Reset The View
    
    glTranslatef(-1.5f,0.0f,-6.0f);               // Move Left 1.5 Units And Into The Screen 6.0
    glRotatef(rot_pyr,1.0f,0.0f,0.0f);
    pyramid( 1.0f, 1.0f, 1.0f);              

    glLoadIdentity();

    glTranslatef(1.5f,0.0f,-6.0f);                 // Move Right 3 Units    
    glRotatef(rot_hex,1.0f,0.0f,0.0f);
    hexagon(1.0f, 1.0f);

    rot_pyr+=0.1f;
    rot_hex+=0.1f;
    // swap buffers to display, since we're double buffered.
    SDL_GL_SwapBuffers();
}


int main(int argc, char **argv)
{
    int done;

    /* Initialize SDL for video output */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Create a 800x600 OpenGL screen */
    if ( SDL_SetVideoMode(scr_w, scr_h, 0, SDL_OPENGL) == NULL )
    {
        fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
        SDL_Quit();
        exit(2);
    }

    /* Set the title bar in environments that support it */
    SDL_WM_SetCaption("Spinning Shapes", NULL);

    /* Loop, drawing and checking events */
    InitGL(scr_w, scr_h);
    done = 0;

    while ( ! done )
    {
        DrawGLScene();

        /* This could go in a separate function */
        {
            SDL_Event event;
            while ( SDL_PollEvent(&event) )
            {
                if ( event.type == SDL_QUIT )
                {
                    done = 1;
                }

                if ( event.type == SDL_KEYDOWN )
                {
                    if ( event.key.keysym.sym == SDLK_ESCAPE )
                    {
                        done = 1;
                    }

                }
            }
        }
    }

    SDL_Quit();
    return 1;
} 
