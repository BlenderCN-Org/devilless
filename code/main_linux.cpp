
#include "stdio.h"
#include "X11/Xlib.h"
#include <X11/Xutil.h>

bool IsRunning = 1;

int main() {
	printf("hello sailor!\n");
	
	Display* display = XOpenDisplay(0);
	
	if (!display) {
		printf("tpc has failed us, we didn't get a window\n");
		return 1;
	}
	
	int rootWindow = DefaultRootWindow(display);
	int defaultScreen = DefaultScreen(display);
	
	XVisualInfo visualInfo = {};
	if (!XMatchVisualInfo(display, defaultScreen, 24, TrueColor, &visualInfo)) {
		printf("No match returned of tpc");
		return 1;
	}
	
	XSetWindowAttributes windowAttributes = {};
	windowAttributes.colormap = XCreateColormap(display, rootWindow, visualInfo.visual, AllocNone);
	windowAttributes.event_mask = StructureNotifyMask;
	
	Window window = XCreateWindow(display, rootWindow, 0, 0, 540, 480, 0, visualInfo.depth, InputOutput, visualInfo.visual, CWBackPixel | CWColormap | CWEventMask, &windowAttributes);
	
	if (!window) {
		printf("tpc request to create window failed");
		return 1;
	}
	
	XStoreName(display, window, "Devilless");
	XMapWindow(display, window);
	XFlush(display);
	
	while(IsRunning) {
		XEvent event = {};
		while (XPending(display) > 0) {
			XNextEvent(display, &event);
			switch(event.type) {
				case DestroyNotify:
				XDestroyWindowEvent *e = (XDestroyWindowEvent *)&event;
				if (e->window == window)
					IsRunning = 0;
				break;
			}
		}
	}
	
	return 0;
}