
#pragma once

#include <GL/gl.h>
#include "common.h"
#include "asset_manager.h"

typedef void glGenBuffers_(GLsizei n, GLuint * buffers);
extern glGenBuffers_ *glGenBuffers;
typedef void glBindBuffer_(GLenum target, GLuint buffer);
extern glBindBuffer_ *glBindBuffer;
typedef void glBufferData_(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
extern glBufferData_ *glBufferData;

typedef void glVertexAttribPointer_(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
extern glVertexAttribPointer_ *glVertexAttribPointer;
typedef void glEnableVertexAttribArray_(GLuint index);
extern glEnableVertexAttribArray_ *glEnableVertexAttribArray;
typedef GLint glGetUniformLocation_(GLuint program,const GLchar *name);
extern glGetUniformLocation_ *glGetUniformLocation;
typedef GLint glGetAttribLocation_(GLuint program, const GLchar *name);
extern glGetAttribLocation_ *glGetAttribLocation;

typedef void glUniformMatrix4fv_(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern glUniformMatrix4fv_ *glUniformMatrix4fv;

typedef GLuint glCreateProgram_(void);
extern glCreateProgram_ *glCreateProgram;
typedef GLuint glCreateShader_(GLenum type);
extern glCreateShader_ *glCreateShader;
typedef void glLinkProgram_(GLuint program);
extern glLinkProgram_ *glLinkProgram;
typedef void glAttachShader_(GLuint program, GLuint shader);
extern glAttachShader_ *glAttachShader;
typedef void glShaderSource_(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
extern glShaderSource_ *glShaderSource;
typedef void glCompileShader_(GLuint shader);
extern glCompileShader_ *glCompileShader;
typedef void glDeleteShader_(GLuint shader);
extern glDeleteShader_ *glDeleteShader;
typedef void glUseProgram_(GLuint program);
extern glUseProgram_ *glUseProgram;

typedef void glGetShaderiv_(GLuint shader, GLenum pname, GLint *params);
extern glGetShaderiv_ *glGetShaderiv;
typedef void glGetShaderInfoLog_(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
extern glGetShaderInfoLog_ *glGetShaderInfoLog;
typedef void glGetProgramiv_(GLuint program, GLenum pname, GLint *params);
extern glGetProgramiv_ *glGetProgramiv;
typedef void glGetProgramInfoLog_(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
extern glGetProgramInfoLog_ *glGetProgramInfoLog;

struct MeshInfo {
	GLuint vertexVBO;
	u32 verticesSize;
	
	GLuint indexVBO;
	u32 indicesSize;
};

struct ShaderInfo {
	GLuint programID;
	GLuint uniformModelView;
	GLuint uniformViewProjection;
	GLuint attribVertexPosition;
	GLuint attribVertexColor;
};

struct RenderState {
	MeshInfo meshInfos[MESH_ID_COUNT];
	ShaderInfo shaderInfos[SHADER_ID_COUNT];
};

extern RenderState renderState;