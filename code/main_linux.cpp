
#include "common.h"
#include "game_math.h"
#include "main.h"
#include <stdio.h>
#include "main.cpp"

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "main_linux.h"

static XState *xState;

#include "renderer_opengl_x11.cpp"
#include "renderer_opengl.cpp"

bool IsRunning = 1;

void ProcessInput(InputKey *inputKey, bool isDown) {
	if (inputKey->isDown != isDown) {
		inputKey->isDown = isDown;
		inputKey->count++;
	}
}

void ProcessMessages(GameInput *gameInput) {
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

bool InitX() {
	xState->display = XOpenDisplay(0);
	
	if (!xState->display) {
		printf("Error: failed to open X11 display.\n");
		return 0;
	}
	
	xState->rootWindow = DefaultRootWindow(xState->display);
	xState->defaultScreen = DefaultScreen(xState->display);
	
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
	XVisualInfo* visualInfo = glXChooseVisual(xState->display, xState->defaultScreen, glxAttribs);
	
	if (!visualInfo) {
		printf("Error: X11 couldn't match requested visual info.\n");
		return 0;
	}
		
	XSetWindowAttributes windowAttributes = {};
	windowAttributes.colormap = XCreateColormap(xState->display, xState->rootWindow, visualInfo->visual, AllocNone);
	windowAttributes.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask;
	
	xState->window = XCreateWindow(xState->display, xState->rootWindow, 0, 0, 540, 480, 0, visualInfo->depth, InputOutput, visualInfo->visual, CWBackPixel | CWColormap | CWEventMask, &windowAttributes);
	
	if (!xState->window) {
		printf("Error: failed to create window.\n");
		return 0;
	}
	
	xState->glContext = glXCreateContext(xState->display, visualInfo, NULL, GL_TRUE);
	glXMakeCurrent(xState->display, xState->window, xState->glContext);
	
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
	
	xState = new XState();
	if (!InitX())
		return 1;
	
	InitOpenGL();
	
	GameInput *gameInput = Alloc(GameInput);
	
	GameInit(gameInput);
	InitShader();
	InitMesh();
	
	gameInput->keyMap[KeyPause] = GetKeyCode(xState, '\t');
	gameInput->keyMap[KeyUp] = GetKeyCode(xState, 'W');
	
	while (IsRunning) {
		for (u32 i = 0; i < ArrayCount(gameInput->key); i++)
		{
			gameInput->key[i].count = 0;
		}
		
		ProcessMessages(gameInput);
		
		/*if (activeWindow)
		{
			gameInput->mousePos = ;
		}
		else
		{
			ClearInput();
		}*/
		
		GameUpdate(gameInput);
		
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.4f, 0.1f, 0.13f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, 540, 480);
		
		/*glBegin(GL_TRIANGLES);
		glColor3f(  1.0f,  0.0f, 0.0f);
		glVertex3f( 0.0f, -1.0f, 0.0f);
		glColor3f(  0.0f,  1.0f, 0.0f);
		glVertex3f(-1.0f,  1.0f, 0.0f);
		glColor3f(  0.0f,  0.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f, 0.0f);
		glEnd();*/
		
		RenderMesh();
		
		glXSwapBuffers(xState->display, xState->window);
	}
	
	glXDestroyContext(xState->display, xState->glContext);
	XDestroyWindow(xState->display, xState->window);
	
	return 0;
}