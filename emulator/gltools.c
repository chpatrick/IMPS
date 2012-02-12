#include "gltools.h"

SDL_Surface * sdlSurface;

void glEvent(SDL_Event * event) 
{ 
  switch(event->type)
  {
    case SDL_QUIT:
      exit(0);
      break;
    case SDL_KEYDOWN:
      switch(event->key.keysym.sym)
      {
        case SDLK_ESCAPE:
          exit(0);
          break;
        default:
          break;
      }
      break;
  }
}

void glStartFrame()
{
  SDL_Event event;

  while(SDL_PollEvent(&event))
  {
    glEvent(&event);
  }
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the lovely buffer
}

void gl3dProjection()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective( 45.0f,
                  (GLfloat)GL_WIDTH/(GLfloat)GL_HEIGHT,
                  0.1f,
                  1000.0f);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  
  glTranslatef(-0.0f,0.0f,-6.0f);

  glEnable(GL_DEPTH_TEST);
}

void gl2dProjection()
{
  glMatrixMode (GL_PROJECTION);

  glLoadIdentity();

  glOrtho (0, GL_WIDTH, GL_HEIGHT, 0, 0, 1);

  glMatrixMode (GL_MODELVIEW);
  
  glLoadIdentity();

  glTranslatef(0.5f, 0.5f, 0);

  glDisable(GL_DEPTH_TEST);
}
 
void initOpenGL()	  
{
  int width = GL_WIDTH;  
  int height = GL_HEIGHT;
  
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) 
  {
    return;
  }

  if((sdlSurface = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL)) == NULL) 
  {
    return;
  }

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);


  SDL_WM_SetCaption("OpenGL Render",NULL);  

  glClearDepth(1.0);
  glDepthFunc(GL_LESS);	
  glShadeModel(GL_SMOOTH);
  
  glViewport(0, 0, height, width);
  glEnable(GL_TEXTURE_2D);
  //glPointSize(2.0f);
  
  reSizeScene(width, height); 
}

void reSizeScene(int width, int height)
{
  glViewport(0, 0, width, height);
  
  if(height != 0)
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective( 45.0f,
                    (GLfloat)width/(GLfloat)height,
                    0.1f,
                    100.0f);

    glMatrixMode(GL_MODELVIEW);
  }
}

#ifdef OPENGLTOOLS_ISOLATED

// This code is for testing purposes

int main(int argc, char** argv)
{
  initOpenGL();
  for(;;)
  {
    glStartFrame();
    gl2dProjection();

    vertex v0 = {0.0f,1.0f,0.0f,
                 1.0f,1.0f,0.0f};

    vertex v1 = {0.0f,0.0f,0.0f,
                 1.0f,0.0f,1.0f};

    vertex v2 = {1.0f,1.0f,0.0f,
                 1.0f,1.0f,1.0f};

    vertex v3 = {1.0f,0.0f,0.0f,
                 1.0f,1.0f,1.0f};
    
    r[20] = (word) 100;
    r[21] = (word) 100;
    r[24] = (word) 0xFFFFFF;
    glDrawPixelRGB();

    glPresentFrame();
  }
  return 0;
}

#endif
