
#include "common.h"
#include <GL/glx.h>

struct XState {
	Display *display;
	i32 rootWindow;
	i32 defaultScreen;
	Window window;
	
	Atom atomDeleteWindow;
	
	// TODO: move this to opengl x11
	GLXContext glContext;
};

extern XState xState;