//#pragma once

#ifndef GPUOPS_H
#define GPUOPS_H

#include <GL/gl.h>
#include <SDL/SDL.h>
#include "hardware.h"
#include "gltools.h"

//#define glPresentFrame SDL_GL_SwapBuffers

void initOpenGL();
void glStartFrame();
void gl3dProjection();
void gl2dProjection();
void glStartPoly();
void glStartPoint();
void glDrawVertex();
void glDrawPixelRGB();
void glPresentFrame();


typedef void (*gpuptr) (void);

gpuptr gpucalls[] = {
  initOpenGL,       //0
  glStartFrame,     //1
  gl3dProjection,   //2
  gl2dProjection,   //3
  glStartPoly,      //4
  glDrawVertex,     //5
  glDrawPixelRGB,   //6
  glEnd,            //7
  glPresentFrame    //8
};

void glStartPoly()
{
  glBegin(GL_POLYGON);
}

void glStartPoint()
{
  glBegin(GL_POINTS);
}

void glDrawPixelRGB()
{
  //Must be preceded by gl2dProjection
  //glDrawPixel doesn't require glEnd or any glStart
  //Can be configure to draw sprites

  glRasterPos2f(r[20].f,r[21].f);
  glDrawPixels(1, 1, GL_RGB, GL_FLOAT, (float*) &r[23].f);
  glRasterPos2i(0,0);
}

void glDrawVertex()
{
  //Must be preceded by gl3dProjection
  #ifdef DEBUG
  printf("r float = ");
  printf("%f",r[23].f);
  printf("\n");

	printf("g float = ");
  printf("%f",r[24].f);
  printf("\n");

  printf("b float = ");
  printf("%f",r[25].f);
  printf("\n");
  printf("\n");
  #endif  

  glVertex3f(r[20].f,r[21].f,r[22].f);
  glColor3f(r[23].f,r[24].f,r[25].f);
}

void glPresentFrame()
{
  SDL_GL_SwapBuffers();
  SDL_Delay(0);
}
#endif

