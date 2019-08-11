
#include "common.h"
#include <GL/glx.h>

struct XState {
	Display *display;
	i32 rootWindow;
	i32 defaultScreen;
	Window window;
	
	Atom atomDeleteWindow;
	
	GLXContext glContext;
};

extern XState *xState;