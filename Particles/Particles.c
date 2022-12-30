#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <time.h>

/* Draw a frame every 20ms */
#define FRAME_INTERVAL 20

/* Number of particles to animate */
#define PARTICLES 150

/* Screen Settings */
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define SCREEN_DEPTH  24

GLuint texture;                                                                /* Storage For Our Particle Texture */
SDL_Surface *TextureImage;
unsigned int ogl15 = 0;

enum TIMERS {
    TIM_FRAME_INTERVAL
};

struct particle {
    float x;
    float y;
    float z;
    float xspeed;
    float yspeed;
    float size;
    unsigned int r;
    unsigned int g;
    unsigned int b;
};

struct particle particles[PARTICLES];

/* Try to exit cleanly */
int _exit_(int code) {
    if(SDL_WasInit(SDL_INIT_VIDEO) != 0) {

        glDeleteTextures(1, &texture);                                         /* Clean up our textures */
        SDL_Quit();
    }

    exit(code);
}

/* Generate a random integer between low and high inclusive */
int genRandInt(int low, int high) {
    static int seed = 1;
    int num;

    if (seed == 1) {                                                           /* Gets called only once as seed is static */
        srand(time(NULL));
        seed = 0;
    }

    num = (rand() % (high - low + 1) + low);                                   /* Generate number between limits, inclusive */
    return (num);
}

/* Generate a random float between low and high inclusive */
float genRandFloat(float low, float high) {
    static int seed = 1;
    float num;

    if (seed == 1) {                                                           /* Gets called only once as seed is static */
        srand(time(NULL));
        seed = 0;
    }

    num = (rand() / ((float)(RAND_MAX) + 1.0)) * (high - low) + low;
    return num;
}

void initfountainFX(void) {
    int i;
    int res;

    for (i = 0 ; i < PARTICLES ; i++) {
        particles[i].x = 0.0f;
        particles[i].y = -0.5f;
        particles[i].xspeed = genRandFloat(-0.02f, 0.02f);
        particles[i].yspeed = genRandFloat(0.02f, 0.05f);
        particles[i].size = genRandFloat(10.0f, 32.0f);
        particles[i].r = 255;
        particles[i].g = 255;
        particles[i].b = 255;
        glColor3ub(particles[i].r, particles[i].g, particles[i].b);
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -1.1f);
}

void fountainFX(void) {
    int i;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (i = 0; i < PARTICLES ; i++) {
        particles[i].yspeed -= 0.0007f;
        particles[i].y += particles[i].yspeed;
        particles[i].x += particles[i].xspeed;

        if (particles[i].y < -0.5f) {
            particles[i].x = 0.0f;
            particles[i].y = -0.5f;
            particles[i].xspeed = genRandFloat(-0.02f, 0.02f);
            particles[i].yspeed = genRandFloat(0.02f, 0.05f);
            particles[i].r = 255;
            particles[i].g = 255;
            particles[i].b = 255;
            glColor3ub(particles[i].r, particles[i].g, particles[i].b);
        }

        if (particles[i].r > 0) particles[i].r = particles[i].r-5;
        if (particles[i].g > 0) particles[i].g = particles[i].g-5;
        if (particles[i].b > 0) particles[i].b = particles[i].b-5;

        glPointSize(particles[i].size);
        glBegin(GL_POINTS);
        glColor3ub(particles[i].r, particles[i].g, particles[i].b);
        glVertex3f(particles[i].x, particles[i].y, particles[i].z);
        glEnd();
    }

    SDL_GL_SwapBuffers();
}

/* Timer callback. Create a redraw event */
Uint32 _frametimer(Uint32 interval, void *param) {
    SDL_Event event;

    /* Don't overload queue with draw events and delaying other events */
    if (SDL_PeepEvents(NULL, 1, SDL_PEEKEVENT, SDL_ALLEVENTS) == 0) {
        event.type = SDL_USEREVENT;
        event.user.code = TIM_FRAME_INTERVAL;
        SDL_PushEvent(&event);
    }

    return interval;
}

void initTextures(void) {
    int res;

    if ((TextureImage = SDL_LoadBMP("star.bmp"))) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage->w, TextureImage->h, 0, GL_BGR, GL_UNSIGNED_BYTE, TextureImage->pixels);    /* Generate The Texture */
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );     /* Linear Filtering */
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );     /* Linear Filtering */
    }

    if (TextureImage) {     /* Free up any memory we may have used */
        SDL_FreeSurface(TextureImage);
    }
}

/* Initialise OpenGL */
void initOpenGL(int scr_w, int scr_h) {
    glViewport(0, 0, scr_w, scr_h);

    if ( gluCheckExtension((const GLubyte*) "GL_ARB_point_sprite", glGetString(GL_EXTENSIONS)) == GLU_TRUE) {
        ogl15 = 1;
    }

    initTextures();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat) scr_w / (GLfloat) scr_h,  0.0f, 2.0f); /* Calc Aspect Ratio */
    glMatrixMode(GL_MODELVIEW);

    if (ogl15 == 1) {
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_POINT_SPRITE);
        glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else { /* Fallback if we can't use point sprites */
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_POINT_SMOOTH);
        glColor3ub(genRandInt(100,255), genRandInt(100,255), genRandInt(100,255));
    }
}

void mainloop(int scr_w, int scr_h, int scr_d) {
    SDL_TimerID timerMain;
    SDL_Event event;
    SDL_Surface* mainSurface;
    int _quit = 0;

    /* Initialise SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        _exit_(1);
    }

    SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );

    /* Some SDL installations don't default to desktop display size if 0 is passed */
    /* so if we fail, try with some defaults */
    if ((mainSurface = SDL_SetVideoMode(scr_w, scr_h, scr_d, SDL_OPENGL | SDL_RESIZABLE)) == NULL) {
        scr_w = 640;
        scr_h = 480;
        if ((mainSurface = SDL_SetVideoMode(scr_w, scr_h, scr_d, SDL_OPENGL | SDL_RESIZABLE)) == NULL) {
            fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
            _exit_(2);
        }
    }

    SDL_WM_SetCaption("Particles", "Particles");
    initOpenGL(scr_w, scr_h);   /* Init OpenGL view port */
    timerMain = SDL_AddTimer(FRAME_INTERVAL, _frametimer, NULL);    /* Init framerate timer */
    initfountainFX();

    while ( SDL_WaitEvent(&event) && _quit == 0) {
        switch (event.type) {
            case SDL_QUIT: _quit = 1; break;
            case SDL_USEREVENT:
                if (event.user.code == TIM_FRAME_INTERVAL) {
                    fountainFX();
                }
                break;
            case SDL_KEYDOWN: _quit = 1; break;
        }
    }

    SDL_Quit();
}

int main(int argc, char **argv) {
    unsigned int scr_w = SCREEN_WIDTH;
    unsigned int scr_h = SCREEN_HEIGHT;
    unsigned int scr_d = SCREEN_DEPTH;

    mainloop(scr_w, scr_h, scr_d);
    _exit_(0);
    return 0; /* Silence Warnings */
}
