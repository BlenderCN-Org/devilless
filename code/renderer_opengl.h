
#pragma once

#include <GL/gl.h>
#include "common.h"
#include "asset_manager.h"

typedef void _glGenBuffers(GLsizei n, GLuint * buffers);
extern _glGenBuffers *glGenBuffers;
typedef void _glBindBuffer(GLenum target, GLuint _buffer);
extern _glBindBuffer *glBindBuffer;
typedef void _glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
extern _glBufferData *glBufferData;

typedef void _glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
extern _glVertexAttribPointer *glVertexAttribPointer;
typedef void _glEnableVertexAttribArray(GLuint _index);
extern _glEnableVertexAttribArray *glEnableVertexAttribArray;
typedef GLint _glGetUniformLocation(GLuint _program,const GLchar *name);
extern _glGetUniformLocation *glGetUniformLocation;
typedef GLint _glGetAttribLocation(GLuint _program, const GLchar *name);
extern _glGetAttribLocation *glGetAttribLocation;

typedef void _glUniformMatrix4fv(GLint _location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern _glUniformMatrix4fv *glUniformMatrix4fv;

typedef GLuint _glCreateProgram(void);
extern _glCreateProgram *glCreateProgram;
typedef GLuint _glCreateShader(GLenum type);
extern _glCreateShader *glCreateShader;
typedef void _glLinkProgram(GLuint _program);
extern _glLinkProgram *glLinkProgram;
typedef void _glAttachShader(GLuint _program, GLuint _shader);
extern _glAttachShader *glAttachShader;
typedef void _glShaderSource(GLuint _shader, GLsizei count, const GLchar *const*string, const GLint *length);
extern _glShaderSource *glShaderSource;
typedef void _glCompileShader(GLuint _shader);
extern _glCompileShader *glCompileShader;
typedef void _glDeleteShader(GLuint _shader);
extern _glDeleteShader *glDeleteShader;
typedef void _glUseProgram(GLuint _program);
extern _glUseProgram *glUseProgram;

typedef void _glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
extern _glGetShaderiv *glGetShaderiv;
typedef void _glGetShaderInfoLog(GLuint _shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
extern _glGetShaderInfoLog *glGetShaderInfoLog;
typedef void _glGetProgramiv(GLuint program, GLenum pname, GLint *params);
extern _glGetProgramiv *glGetProgramiv;
typedef void _glGetProgramInfoLog(GLuint _program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
extern _glGetProgramInfoLog *glGetProgramInfoLog;

struct MeshInfo {
	u32 indexCount;
	
	GLuint vertexVBO;
	GLuint indexVBO;
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
	MeshInfo skinInfos[SKIN_ID_COUNT];
	ShaderInfo shaderInfos[SHADER_ID_COUNT];
};

extern RenderState renderState;