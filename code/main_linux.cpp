
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#include "main_linux.h"
#include "main.h"
#include "renderer.h"
#include "game_math.h"
#include "memory.h"

XState xState = {};
bool IsRunning = 1;

MemoryInfo Alloc(uSize size) {
	MemoryInfo mi = {};
	
	mi.base = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	mi.size = size;
	
	return mi;
}

uSize ReadFile(void *base, char *fileName) {
	int fileHandle = open(fileName, O_RDONLY);
	if (fileHandle == -1)
		return 0;
	
	struct stat fileStatus;
	if (fstat(fileHandle, &fileStatus) == -1)
	{
		close(fileHandle);
		return 0;
	}
	
	if (fileStatus.st_size <= 0) {
		close(fileHandle);
		return 0;
	}
	
	uSize bytesLeft = fileStatus.st_size;
	u8 *tail = (u8 *)base;
	while (bytesLeft)
	{
		iSize bytesRead = read(fileHandle, tail, bytesLeft);
		if (bytesRead == -1)
		{
			close(fileHandle);
			return 0;
		}
		bytesLeft -= bytesRead;
		tail += bytesRead;
	}
	
	close(fileHandle);
	return fileStatus.st_size;
}

v2i GetScreenSize() {
	return xState.screenSize;
}

void InitInput(GameInput *gameInput) {
	// TODO: load imputs from file, if no file exists create one with default inputs
	gameInput->keyMap[KeyUp] = XKeysymToKeycode(xState.display, 'W');
	gameInput->keyMap[KeyDown] = XKeysymToKeycode(xState.display, 'S');
	gameInput->keyMap[KeyLeft] = XKeysymToKeycode(xState.display, 'A');
	gameInput->keyMap[KeyRight] = XKeysymToKeycode(xState.display, 'D');
	gameInput->keyMap[KeyRun] = XKeysymToKeycode(xState.display, XK_Shift_L);
	gameInput->keyMap[KeyPause] = XKeysymToKeycode(xState.display, XK_Escape);
}

void ClearInput(GameInput *gameInput) {
	for (u32 i = 0; i < ArrayCount(gameInput->keys); i++) {
		gameInput->keys[i].count = 0;
		gameInput->keys[i].isDown = 0;
	}
}

void ProcessInput(InputKey *inputKey, bool isDown) {
	if (inputKey->isDown != isDown) {
		inputKey->isDown = isDown;
		inputKey->count++;
	}
}

void ProcessMessages(GameInput *gameInput) {
	XEvent event = {};
	while (XPending(xState.display) > 0) {
		XNextEvent(xState.display, &event);
		switch (event.type) {
			case DestroyNotify:
			{
				XDestroyWindowEvent *e = (XDestroyWindowEvent *)&event;
				if (e->window == xState.window)
					IsRunning = 0;
				break;
			}
			
			case ClientMessage:
			{
				XClientMessageEvent *e = (XClientMessageEvent *)&event;
				if ((Atom)e->data.l[0] == xState.atomDeleteWindow)
					IsRunning = 0;
				break;
			}
			
			case ConfigureNotify:
			{
				XConfigureEvent *e = (XConfigureEvent *)&event;
				xState.screenSize = V2I(e->width, e->height);
				printf("received configure notification: %d, %d\n", e->width, e->height);
			}
			
			case KeyPress:
			{
				XKeyPressedEvent *e = (XKeyPressedEvent *)&event;
				ProcessInput(&gameInput->keys[e->keycode], 1);
				break;
			}
			
			case KeyRelease:
			{
				XKeyPressedEvent *e = (XKeyPressedEvent *)&event;
				ProcessInput(&gameInput->keys[e->keycode], 0);
				break;
			}
		}
	}
}

bool InitX() {
	XInitThreads();
	xState.display = XOpenDisplay(0);
	
	if (!xState.display) {
		printf("Error: failed to open X11 display.\n");
		return 0;
	}
	
	xState.rootWindow = DefaultRootWindow(xState.display);
	xState.screenNumber = DefaultScreen(xState.display);
	
	GLint glxAttribs[] = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_DEPTH_SIZE,     24,
		GLX_STENCIL_SIZE,   8,
		GLX_RED_SIZE,       8,
		GLX_GREEN_SIZE,     8,
		GLX_BLUE_SIZE,      8,
		GLX_SAMPLE_BUFFERS, 0,
		GLX_SAMPLES,        0,
		None
	};
	XVisualInfo* visualInfo = glXChooseVisual(xState.display, xState.screenNumber, glxAttribs);
	
	if (!visualInfo) {
		printf("Error: X11 couldn't match requested visual info.\n");
		return 0;
	}
		
	XSetWindowAttributes windowAttributes = {};
	windowAttributes.colormap = XCreateColormap(xState.display, xState.rootWindow, visualInfo->visual, AllocNone);
	windowAttributes.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask;
	
	xState.window = XCreateWindow(xState.display, xState.rootWindow, 0, 0, 540, 480, 0, visualInfo->depth, InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &windowAttributes);
	
	if (!xState.window) {
		printf("Error: failed to create window.\n");
		return 0;
	}
	
	xState.glContext = glXCreateContext(xState.display, visualInfo, NULL, GL_TRUE);
	glXMakeCurrent(xState.display, xState.window, xState.glContext);
	
	XStoreName(xState.display, xState.window, "Devilless");
	XMapWindow(xState.display, xState.window);
	XFlush(xState.display);
	
	xState.atomDeleteWindow = XInternAtom(xState.display, "WM_DELETE_WINDOW", False);
	if (!XSetWMProtocols(xState.display, xState.rootWindow, &xState.atomDeleteWindow, 1)) {
		printf("Warning: request to handle window deletion failed.\n");
	}
	
	return 1;
}

int main() {
	printf("hello sailor!\n");
	
	if (!InitX())
		return 1;
	
	
	GameStack mainStack = {};
	GameInput gameInput = {};
	TempMemory tempMemory = {};
	
	InitStack(&mainStack, Alloc(Megabytes(600)));
	InitStack(&tempMemory.stack, Alloc(Megabytes(150)));
	
	InitRenderer(&tempMemory);
	InitInput(&gameInput);
	GameInit(&mainStack, &tempMemory);
	
	timeval timeVal;
	gettimeofday(&timeVal, 0);
	u64 frameStartCounter = timeVal.tv_sec * 1000000 + timeVal.tv_usec;
	
	f32 deltaTime = 0;
	
	while (IsRunning) {
		for (u32 i = 0; i < ArrayCount(gameInput.keys); i++)
		{
			gameInput.keys[i].count = 0;
		}
		
		ProcessMessages(&gameInput);
		
		/*if (activeWindow)
		{
			gameInput->mousePos = ;
		}
		else
		{
			ClearInput();
		}*/
		
		ClearFrame();
		
		GameUpdate(&mainStack, &gameInput, deltaTime, &tempMemory);
		
		PresentFrame();
		
		gettimeofday(&timeVal, 0);
		const u64 frameFinishCounter = timeVal.tv_sec * 1000000 + timeVal.tv_usec;
		
		u64 counterElapsed = frameFinishCounter - frameStartCounter;
		deltaTime = Min(0.05f, (f32)counterElapsed / 1000000);
		//printf("%f\n", gameState.deltaTime);
		
		frameStartCounter = frameFinishCounter;
	}
	
	glXDestroyContext(xState.display, xState.glContext);
	XDestroyWindow(xState.display, xState.window);
	
	return 0;
}