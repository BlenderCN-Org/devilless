
#pragma once

#include <GL/gl.h>

typedef void glGenBuffers_(GLsizei n, GLuint * buffers);
static glGenBuffers_ *glGenBuffers;
typedef void glBindBuffer_(GLenum target, GLuint buffer);
static glBindBuffer_ *glBindBuffer;
typedef void glBufferData_(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
static glBufferData_ *glBufferData;

typedef void glVertexAttribPointer_(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
static glVertexAttribPointer_ *glVertexAttribPointer;
typedef void glEnableVertexAttribArray_(GLuint index);
static glEnableVertexAttribArray_ *glEnableVertexAttribArray;
typedef GLint glGetUniformLocation_(GLuint program,const GLchar *name);
static glGetUniformLocation_ *glGetUniformLocation;
typedef GLint glGetAttribLocation_(GLuint program, const GLchar *name);
static glGetAttribLocation_ *glGetAttribLocation;

typedef void glUniformMatrix4fv_(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
static glUniformMatrix4fv_ *glUniformMatrix4fv;

typedef GLuint glCreateProgram_(void);
static glCreateProgram_ *glCreateProgram;
typedef GLuint glCreateShader_(GLenum type);
static glCreateShader_ *glCreateShader;
typedef void glLinkProgram_(GLuint program);
static glLinkProgram_ *glLinkProgram;
typedef void glAttachShader_(GLuint program, GLuint shader);
static glAttachShader_ *glAttachShader;
typedef void glShaderSource_(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
static glShaderSource_ *glShaderSource;
typedef void glCompileShader_(GLuint shader);
static glCompileShader_ *glCompileShader;
typedef void glDeleteShader_(GLuint shader);
static glDeleteShader_ *glDeleteShader;
typedef void glUseProgram_(GLuint program);
static glUseProgram_ *glUseProgram;

typedef void glGetShaderiv_(GLuint shader, GLenum pname, GLint *params);
static glGetShaderiv_ *glGetShaderiv;
typedef void glGetShaderInfoLog_(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
static glGetShaderInfoLog_ *glGetShaderInfoLog;
typedef void glGetProgramiv_(GLuint program, GLenum pname, GLint *params);
static glGetProgramiv_ *glGetProgramiv;
typedef void glGetProgramInfoLog_(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
static glGetProgramInfoLog_ *glGetProgramInfoLog;