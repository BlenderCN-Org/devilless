
#include <GL/glx.h>

typedef void glXSwapIntervalEXT_(Display *dpy, GLXDrawable drawable, int interval);
extern glXSwapIntervalEXT_ *glXSwapIntervalEXT;
typedef int glXSwapIntervalMESA_(unsigned int interval);
extern glXSwapIntervalMESA_ *glXSwapIntervalMESA;
typedef int glXSwapIntervalSGI_(int interval);
extern glXSwapIntervalSGI_ *glXSwapIntervalSGI;

void SetVSync(bool on);