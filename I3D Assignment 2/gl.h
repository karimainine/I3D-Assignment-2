#ifndef MYGL_H
#define MYGL_H

#ifdef __cplusplus
extern "C" {
#endif

// So that things will still work on windows
#if WIN32
#include <Windows.h>
#endif

// GL headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#ifdef __cplusplus
}
#endif

#endif
