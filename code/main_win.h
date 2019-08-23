
#include "common.h"

struct WinState {
	HINSTANCE instance;
	HWND window;
	HDC windowDC;
	
	v2i screenSize;
};

extern WinState winState;