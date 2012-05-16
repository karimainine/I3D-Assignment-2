#ifndef MYGL_H
#define MYGL_H

#ifdef __cplusplus
extern "C" {
#endif

// So that things will still work on windows
#if WIN32
#include <Windows.h>
#endif

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#ifdef __cplusplus
}
#endif

#endif
