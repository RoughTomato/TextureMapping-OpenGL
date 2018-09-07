#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "SDL.h"
#include "SDL_image.h"

#define RECTS 2
#define COORDSETS 4
#define TEXTURES 2

GLuint texture[TEXTURES];

typedef struct Rect {
    GLfloat x[COORDSETS];
    GLfloat y[COORDSETS];
} Rect;

Rect rects[RECTS] = {
    {{-2, -2, -1, -1}, {1.0, -1.0, -1.0, 1.0}},
    {{-3.5, -3.5, -2.5, -2.5}, {1.0, -1.0, -1.0, 1.0}}
};

SDL_Surface *images[TEXTURES];
const char *imageList[TEXTURES] = { "images/wick.jpg", "images/bladerunner.jpg" };

void loadImages(void) {
    for(int i = 0; i < TEXTURES; i++){
        images[i] = IMG_Load(imageList[i]);
        if(!images) {
          printf("ERROR: couldn't load image %d\n", i);
          exit(-1);
        }
    }
}

void init(void) {
    glClearColor (0.5, 0.5, 0.5, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    loadImages();
    glGenTextures(2, texture);

    for(int i = 0; i < TEXTURES; i++){
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, images[i]->w, images[i]->h, 0,
        GL_RGB, GL_UNSIGNED_BYTE, images[i]->pixels);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    }

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0; i < TEXTURES; i++) {
        if(i < RECTS) {
            glBindTexture(GL_TEXTURE_2D, texture[i]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(rects[i].x[0],
                rects[i].y[0], 0.0f);
            glTexCoord2f(0.0, 1.0);
            glVertex3f(rects[i].x[1],
                rects[i].y[1], 0.0f);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(rects[i].x[2],
                rects[i].y[2], 0.0f);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(rects[i].x[3],
                rects[i].y[3], 0.0f);
            glEnd();
        }
    }
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0*(GLfloat)w/(GLfloat)h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);
}

void freeSurfaces(void) {
        for(int i = 0; i < TEXTURES; i++) {
            SDL_FreeSurface(images[i]);
        }
}

void keyboard (unsigned char key, int x, int y){

    switch (key) {
       case 27: //“ESC” to exit the program.
       freeSurfaces();
       exit(0);
       break;
        default:
        break;
    }
}

GLvoid timer( int value ) {
    for(int i = 0; i < RECTS; i++) {
        for(int j = 0; j < COORDSETS; j++){
            rects[i].x[j] += 0.02;
            if(rects[i].x[j] > 4) {
                rects[i].x[0] = -4;
                rects[i].x[1] = -4;
                rects[i].x[2] = -3;
                rects[i].x[3] = -3;
            }
        }
    }
   glutPostRedisplay();
   glutTimerFunc(40,timer,value);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("GL Texture mapping");
    init();
    glutPostRedisplay();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(40,timer,0);
    glutMainLoop();
    return 0;
}

