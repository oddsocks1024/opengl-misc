#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>
#include <SDL/SDL.h>

#define N_TEXTURES 6
const int scr_w = 1440;
const int scr_h = 900;
float      xrot = 0.0f;
float      yrot = 0.0f;
float      zrot = 0.0f;

GLuint texture[N_TEXTURES];

SDL_Surface *LoadBMP(char *filename)
{
    printf("Loading BMP: %s\n", filename);
    SDL_Surface *bmp;
    bmp = SDL_LoadBMP(filename);

    if (bmp == NULL)
    {
        fprintf(stderr, "Unable to load %s: %s\n", filename, SDL_GetError());
        return(NULL);
    }

    return(bmp);
}

void LoadGLTextures(void)
{
    char *tex_fnames[N_TEXTURES];
    SDL_Surface *img[N_TEXTURES];
    int i = 0;

    tex_fnames[0] = "unionflag.bmp";
    tex_fnames[1] = "australianflag.bmp";
    tex_fnames[2] = "canadianflag.bmp";
    tex_fnames[3] = "newzealandflag.bmp";
    tex_fnames[4] = "englandflag.bmp";
    tex_fnames[5] = "germanflag.bmp";

    // Create 3 Texture Names	
    glGenTextures(N_TEXTURES, &texture[0]);

    for (i = 0; i < N_TEXTURES; i++)
    {
        img[i] = LoadBMP(tex_fnames[i]);
        glBindTexture(GL_TEXTURE_2D, texture[i]);   // 2d texture (x and y size)
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 3, img[i]->w, img[i]->h, 0, GL_BGR, GL_UNSIGNED_BYTE, img[i]->pixels);
    }
}



void InitGL(int Width, int Height)              // We call this right after our OpenGL window is created.
{
    glViewport(0, 0, Width, Height);
    LoadGLTextures();
    glEnable(GL_TEXTURE_2D);
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
             
    glTranslatef(0.0f,0.0f,-5.0f);              // move 5 units into the screen.

    glRotatef(xrot,1.0f,0.0f,0.0f);             // Rotate On The X Axis
    glRotatef(yrot,0.0f,1.0f,0.0f);             // Rotate On The Y Axis
    glRotatef(zrot,0.0f,0.0f,1.0f);             // Rotate On The Z Axis

    glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.

    glBegin(GL_QUADS);                          // begin drawing a cube

    // Front Face (note that the texture's corners have to match the quad's corners)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[1]);   // choose the texture to use.

    glBegin(GL_QUADS);

    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[2]);   // choose the texture to use.

    glBegin(GL_QUADS);    

    // Top Face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[3]);   // choose the texture to use.

    glBegin(GL_QUADS);

    // Bottom Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[4]);   // choose the texture to use.

    glBegin(GL_QUADS);

    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[5]);   // choose the texture to use.

    glBegin(GL_QUADS);

    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad

    glEnd();                                    // done with the polygon.

    xrot+=0.07f;                         // X Axis Rotation
    yrot+=0.05f;                    // Y Axis Rotation
    zrot+=0.03f;                         // Z Axis Rotation



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
    if ( SDL_SetVideoMode(scr_w, scr_h, 32, SDL_OPENGL | SDL_FULLSCREEN) == NULL )
    {
        fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
        SDL_Quit();
        exit(2);
    }

    /* Set the title bar in environments that support it */
    SDL_WM_SetCaption("Ian Chapman : Flag Cube", NULL);

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
                    
                    //SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
                }
            }
        }
    }

    SDL_Quit();
    return 1;
} 
