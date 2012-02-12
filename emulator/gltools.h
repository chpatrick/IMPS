#ifndef OPENGLFUNCTIONS_H
#define OPENGLFUNCTIONS_H

//#define OPENGLTOOLS_ISOLATED

//#include "gpuops.h"
#include <GL/gl.h>
#include <SDL/SDL.h>

#define GL_WIDTH 640
#define GL_HEIGHT 480

typedef struct vertex
{
  GLfloat x, y, z;
  GLfloat r, g, b;
} vertex;

void glStartFrame();
void initOpenGL();
void gl3dProjection();

void reSizeScene(int width, int height);
void glEvent(SDL_Event * event);

#endif
