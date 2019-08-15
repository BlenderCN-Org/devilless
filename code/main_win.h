
#include "common.h"

struct WinState {
	HINSTANCE instance;
	HWND window;
	HDC windowDC;
};

extern WinState winState;