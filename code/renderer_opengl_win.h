#pragma once

#include <windows.h>
#include <GL/gl.h>

#define GL_STREAM_DRAW                    0x88E0
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_LINK_STATUS                    0x8B82

typedef bool BOOl;
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef ptrdiff_t GLsizeiptr;
typedef char GLchar;
typedef double GLdouble;
typedef ptrdiff_t GLintptr;

typedef BOOl _wglSwapIntervalExt(int interval);
extern _wglSwapIntervalExt *wglSwapIntervalExt;

void SetVSync(bool on);