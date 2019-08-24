
#include "common.h"
#include <GL/glx.h>

struct XState {
	Display *display;
	i32 rootWindow;
	i32 screenNumber;
	Window window;
	
	Atom atomDeleteWindow;
	
	v2i screenSize;
	
	// TODO: move this to opengl x11
	GLXContext glContext;
};

extern XState xState;