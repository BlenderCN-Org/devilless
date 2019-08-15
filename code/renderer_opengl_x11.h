#pragma once

#include <GL/glx.h>

typedef void _glXSwapIntervalEXT(Display *dpy, GLXDrawable drawable, int interval);
extern _glXSwapIntervalEXT *glXSwapIntervalEXT;
typedef int _glXSwapIntervalMESA(unsigned int interval);
extern _glXSwapIntervalMESA *glXSwapIntervalMESA;
typedef int _glXSwapIntervalSGI(int interval);
extern _glXSwapIntervalSGI *glXSwapIntervalSGI;

void SetVSync(bool on);