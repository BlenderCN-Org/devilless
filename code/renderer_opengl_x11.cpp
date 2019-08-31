
#include <stdio.h>

#include "renderer_opengl_x11.h"
#include "main_linux.h"
#include "renderer_opengl.cpp"

_glXSwapIntervalEXT *glXSwapIntervalEXT;
_glXSwapIntervalMESA *glXSwapIntervalMESA;
_glXSwapIntervalSGI *glXSwapIntervalSGI;

#define GetAddress(name) name = (_##name *)glXGetProcAddress((const GLubyte *)#name)
void InitGLExtensions() {
	GetAddress(glXSwapIntervalEXT);
	GetAddress(glXSwapIntervalMESA);
	GetAddress(glXSwapIntervalSGI);
	
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
	if (glXSwapIntervalEXT)
		glXSwapIntervalEXT(xState.display, xState.window, on);
	
	if (glXSwapIntervalMESA)
		glXSwapIntervalMESA(on);
	
	if (glXSwapIntervalSGI)
		glXSwapIntervalSGI(on);
}

bool InitRenderer(TempMemory *tempMemory) {
	char *GL_version=(char *)glGetString(GL_VERSION);
    char *GL_vendor=(char *)glGetString(GL_VENDOR);
    char *GL_renderer=(char *)glGetString(GL_RENDERER);
	
	printf("ver: %s, ven: %s, ren: %s\n", GL_version, GL_vendor, GL_renderer);
	
	InitGLExtensions();
	SetVSync(1);
	
	InitShaders(tempMemory);
	
	return 1;
}

void PresentFrame() {
	glXSwapBuffers(xState.display, xState.window);
}