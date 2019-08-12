
#include "renderer_opengl_x11.h"
#include "renderer_opengl.h"
#include "main_linux.h"

glXSwapIntervalEXT_ *glXSwapIntervalEXT;
glXSwapIntervalMESA_ *glXSwapIntervalMESA;
glXSwapIntervalSGI_ *glXSwapIntervalSGI;

void InitGlExtensions() {
	glXSwapIntervalEXT = (glXSwapIntervalEXT_ *)glXGetProcAddress((const GLubyte *)"glXSwapIntervalEXT");
	glXSwapIntervalMESA = (glXSwapIntervalMESA_ *)glXGetProcAddress((const GLubyte *)"glXSwapIntervalMESA");
	glXSwapIntervalSGI = (glXSwapIntervalSGI_ *)glXGetProcAddress((const GLubyte *)"glXSwapIntervalSGI");
	
	glBindBuffer = (glBindBuffer_ *)glXGetProcAddress((const GLubyte *)"glBindBuffer");
	glBufferData = (glBufferData_ *)glXGetProcAddress((const GLubyte *)"glBufferData");
	glGenBuffers = (glGenBuffers_ *)glXGetProcAddress((const GLubyte *)"glGenBuffers");
	
	glVertexAttribPointer = (glVertexAttribPointer_ *)glXGetProcAddress((const GLubyte *)"glVertexAttribPointer");
	glEnableVertexAttribArray = (glEnableVertexAttribArray_ *)glXGetProcAddress((const GLubyte *)"glEnableVertexAttribArray");
	glGetUniformLocation = (glGetUniformLocation_ *)glXGetProcAddress((const GLubyte *)"glGetUniformLocation");
	glGetAttribLocation = (glGetAttribLocation_ *)glXGetProcAddress((const GLubyte *)"glGetAttribLocation");
	
	glUniformMatrix4fv = (glUniformMatrix4fv_ *)glXGetProcAddress((const GLubyte *)"glUniformMatrix4fv");
	
	glCreateProgram = (glCreateProgram_ *)glXGetProcAddress((const GLubyte *)"glCreateProgram");
	glCreateShader = (glCreateShader_ *)glXGetProcAddress((const GLubyte *)"glCreateShader");
	glLinkProgram = (glLinkProgram_ *)glXGetProcAddress((const GLubyte *)"glLinkProgram");
	glAttachShader = (glAttachShader_ *)glXGetProcAddress((const GLubyte *)"glAttachShader");
	glShaderSource = (glShaderSource_ *)glXGetProcAddress((const GLubyte *)"glShaderSource");
	glCompileShader = (glCompileShader_ *)glXGetProcAddress((const GLubyte *)"glCompileShader");
	glDeleteShader = (glDeleteShader_ *)glXGetProcAddress((const GLubyte *)"glDeleteShader");
	glUseProgram = (glUseProgram_ *)glXGetProcAddress((const GLubyte *)"glUseProgram");
	
	glGetShaderiv = (glGetShaderiv_ *)glXGetProcAddress((const GLubyte *)"glGetShaderiv");
	glGetShaderInfoLog = (glGetShaderInfoLog_ *)glXGetProcAddress((const GLubyte *)"glGetShaderInfoLog");
	glGetProgramiv = (glGetProgramiv_ *)glXGetProcAddress((const GLubyte *)"glGetProgramiv");
	glGetProgramInfoLog = (glGetProgramInfoLog_ *)glXGetProcAddress((const GLubyte *)"glGetProgramInfoLog");
}

void SetVSync(bool on) {
	if (glXSwapIntervalEXT) {
		glXSwapIntervalEXT(xState->display, xState->window, on);
	}
	else if (glXSwapIntervalMESA) {
		glXSwapIntervalMESA(on);
	}
	else if (glXSwapIntervalSGI) {
		glXSwapIntervalSGI(on);
	}
}

void InitRenderer() {
	renderState = Alloc(RenderState);
	InitGlExtensions();
	SetVSync(1);
}