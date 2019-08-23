
#include <stdio.h>

#include "renderer_opengl_win.h"
#include "main_win.h"
#include "renderer_opengl.cpp"

_wglSwapIntervalExt *wglSwapIntervalExt;

#define GetAddress(name) name = (_##name *)wglGetProcAddress(#name)
void InitGLExtensions() {
	GetAddress(wglSwapIntervalExt);
	
	GetAddress(glBindBuffer);
	GetAddress(glBufferData);
	GetAddress(glGenBuffers);
	
	GetAddress(glVertexAttribPointer);
	GetAddress(glEnableVertexAttribArray);
	GetAddress(glGetUniformLocation);
	GetAddress(glGetAttribLocation);
	
	GetAddress(glUniformMatrix4fv);
	
	GetAddress(glCreateProgram);
	GetAddress(glCreateShader);
	GetAddress(glLinkProgram);
	GetAddress(glAttachShader);
	GetAddress(glShaderSource);
	GetAddress(glCompileShader);
	GetAddress(glDeleteShader);
	GetAddress(glUseProgram);
	
	GetAddress(glGetShaderiv);
	GetAddress(glGetShaderInfoLog);
	GetAddress(glGetProgramiv);
	GetAddress(glGetProgramInfoLog);
}

void SetVSync(bool on) {
	if (wglSwapIntervalExt)
		wglSwapIntervalExt(on);
}

bool InitRenderer() {
	PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    
    i32 pixelFormat = ChoosePixelFormat(winState.windowDC, &pfd);
    SetPixelFormat(winState.windowDC, pixelFormat, &pfd);
    
    HGLRC openGLRC = wglCreateContext(winState.windowDC);
    
    if (!wglMakeCurrent(winState.windowDC, openGLRC))
    {
		printf("Failed to init OpenGL.\n");
		return 0;
    }
	
	InitGLExtensions();
	
	SetVSync(1);
	
	char *GL_version=(char *)glGetString(GL_VERSION);
    char *GL_vendor=(char *)glGetString(GL_VENDOR);
    char *GL_renderer=(char *)glGetString(GL_RENDERER);
	
	printf("ver: %s, ven: %s, ren: %s\n", GL_version, GL_vendor, GL_renderer);
	
	return 1;
}

void PresentFrame() {
	
	SwapBuffers(winState.windowDC);
}