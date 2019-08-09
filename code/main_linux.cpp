
#include "common.h"
#include "game_math.h"
#include "main.h"

#include <stdio.h>
#include "main.cpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main_linux.h"

bool IsRunning = 1;

void ProcessInput(InputKey *inputKey, bool isDown) {
	if (inputKey->isDown != isDown) {
		inputKey->isDown = isDown;
		inputKey->count++;
	}
}

void ProcessMessages(XState *xState, GameInput *gameInput) {
	XEvent event = {};
	while (XPending(xState->display) > 0) {
		XNextEvent(xState->display, &event);
		switch (event.type) {
			case DestroyNotify:
			{
				XDestroyWindowEvent *e = (XDestroyWindowEvent *)&event;
				if (e->window == xState->window)
					IsRunning = 0;
				break;
			}
			case ClientMessage:
			{
				XClientMessageEvent *e = (XClientMessageEvent *)&event;
				if ((Atom)e->data.l[0] == xState->atomDeleteWindow)
					IsRunning = 0;
				break;
			}
			case KeyPress:
			{
				XKeyPressedEvent *e = (XKeyPressedEvent *)&event;
				ProcessInput(&gameInput->key[e->keycode], 1);
				break;
			}
			case KeyRelease:
			{
				XKeyPressedEvent *e = (XKeyPressedEvent *)&event;
				ProcessInput(&gameInput->key[e->keycode], 0);
				break;
			}
		}
	}
}

bool InitX(XState *xState) {
	xState->display = XOpenDisplay(0);
	
	if (!xState->display) {
		printf("Error: failed to open X11 display.\n");
		return 0;
	}
	
	xState->rootWindow = DefaultRootWindow(xState->display);
	xState->defaultScreen = DefaultScreen(xState->display);
	
	XVisualInfo visualInfo = {};
	if (!XMatchVisualInfo(xState->display, xState->defaultScreen, 24, TrueColor, &visualInfo)) {
		printf("Error: X11 couldn't match requested visual info.\n");
		return 0;
	}
	
	XSetWindowAttributes windowAttributes = {};
	windowAttributes.colormap = XCreateColormap(xState->display, xState->rootWindow, visualInfo.visual, AllocNone);
	windowAttributes.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask;
	
	xState->window = XCreateWindow(xState->display, xState->rootWindow, 0, 0, 540, 480, 0, visualInfo.depth, InputOutput, visualInfo.visual, CWBackPixel | CWColormap | CWEventMask, &windowAttributes);
	
	if (!xState->window) {
		printf("Error: failed to create window.\n");
		return 0;
	}
	
	XStoreName(xState->display, xState->window, "Devilless");
	XMapWindow(xState->display, xState->window);
	XFlush(xState->display);
	
	xState->atomDeleteWindow = XInternAtom(xState->display, "WM_DELETE_WINDOW", False);
	if (!XSetWMProtocols(xState->display, xState->rootWindow, &xState->atomDeleteWindow, 1))
		printf("Warning: request to handle window deletion failed.\n");
	
	return 1;
}

int main() {
	printf("hello sailor!\n");
	
	XState *xState = new XState();
	if (!InitX(xState))
		return 1;
	
	GameInput *gameInput = Alloc(GameInput);
	
	GameInit(gameInput);
	
	gameInput->keyMap[KeyPause] = GetKeyCode(xState, '\t');
	gameInput->keyMap[KeyUp] = GetKeyCode(xState, 'W');
	
	while (IsRunning) {
		for (u32 i = 0; i < ArrayCount(gameInput->key); i++)
		{
			gameInput->key[i].count = 0;
		}
		
		ProcessMessages(xState, gameInput);
		
		/*if (activeWindow)
		{
			gameInput->mousePos = ;
		}
		else
		{
			ClearInput();
		}*/
		
		GameUpdate(gameInput);
		
	}
	
	XDestroyWindow(xState->display, xState->window);
	
	return 0;
}