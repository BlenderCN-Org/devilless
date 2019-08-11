
#include <GL/gl.h>
#include <GL/glx.h>

typedef void glXSwapIntervalEXT_(Display *dpy, GLXDrawable drawable, int interval);
static glXSwapIntervalEXT_ *glXSwapIntervalEXT;
typedef int glXSwapIntervalMESA_(unsigned int interval);
static glXSwapIntervalMESA_ *glXSwapIntervalMESA;
typedef int glXSwapIntervalSGI_(int interval);
static glXSwapIntervalSGI_ *glXSwapIntervalSGI;

void SetVSync(bool on);
void InitOpenGL();