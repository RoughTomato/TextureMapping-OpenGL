#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#include "SDL.h"
#include "SDL_image.h"

#define RECTS 2
#define COORDSETS 4

GLuint texture[2];


void init(void) {
    glClearColor (0.5, 0.5, 0.5, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    SDL_Surface *image1 = IMG_Load("images/wick.jpg");
    if (!image1) {
        printf("ERROR: couldn't load image\n");
        exit(-1);
    }

    SDL_Surface *image2 = IMG_Load("images/bladerunner.jpg");
    if (!image1) {
       printf("ERROR: couldn't load image\n");
       exit(-1);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Create Texture
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->w, image1->h, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image1->pixels);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->w, image2->h, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image2->pixels);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
}

typedef struct Rect {
    GLfloat x[COORDSETS];
    GLfloat y[COORDSETS];
} Rect;

Rect rects[RECTS] = {
    {{-2, -2, -1, -1}, {1.0, -1.0, -1.0, 1.0}},
    {{-3.5, -3.5, -2.5, -2.5}, {1.0, -1.0, -1.0, 1.0}}
};

float obj_x = 0.0f;
float obj_y = 0.0f;

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(rects[0].x[0],
        rects[0].y[0], 0.0f);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(rects[0].x[1],
        rects[0].y[1], 0.0f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(rects[0].x[2],
        rects[0].y[2], 0.0f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(rects[0].x[3],
        rects[0].y[3], 0.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glTranslatef( obj_x, obj_y, 0.0f );
    glVertex3f(rects[1].x[0],
        rects[1].y[0], 0.0f);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(rects[1].x[1],
        rects[1].y[1], 0.0f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(rects[1].x[2],
        rects[1].y[2], 0.0f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(rects[1].x[3],
        rects[1].y[3], 0.0f);
    glEnd();
    glutSwapBuffers();
}


void reshape(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0*(GLfloat)w/(GLfloat)h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);
}


void keyboard (unsigned char key, int x, int y){

    switch (key) {
       case 27: //“ESC” to exit the program TODO: free surfaces
       exit(0);
       break;
        default:
        break;
    }
}

int iteration = 0;
int wait = 0;
GLvoid timer( int value )
{
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
    glutInitWindowSize( 600, 600 );
    glutCreateWindow("GL Texture mapping");
    init();
    glutPostRedisplay();
    glutReshapeFunc (reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(40,timer,0);
    glutMainLoop();
    return 0;
}

