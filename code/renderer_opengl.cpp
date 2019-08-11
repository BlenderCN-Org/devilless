
#include "renderer_opengl.h"
#include <GL/gl.h>
#include "stdio.h"
#include "main.h"

glGenBuffers_ *glGenBuffers;
glBindBuffer_ *glBindBuffer;
glBufferData_ *glBufferData;

glVertexAttribPointer_ *glVertexAttribPointer;
glEnableVertexAttribArray_ *glEnableVertexAttribArray;
glGetUniformLocation_ *glGetUniformLocation;
glGetAttribLocation_ *glGetAttribLocation;

glUniformMatrix4fv_ *glUniformMatrix4fv;

glCreateProgram_ *glCreateProgram;
glCreateShader_ *glCreateShader;
glLinkProgram_ *glLinkProgram;
glAttachShader_ *glAttachShader;
glShaderSource_ *glShaderSource;
glCompileShader_ *glCompileShader;
glDeleteShader_ *glDeleteShader;
glUseProgram_ *glUseProgram;

glGetShaderiv_ *glGetShaderiv;
glGetShaderInfoLog_ *glGetShaderInfoLog;
glGetProgramiv_ *glGetProgramiv;
glGetProgramInfoLog_ *glGetProgramInfoLog;

static GLuint vertex_vbo;
static GLuint index_vbo;
static GLuint debugShader;
static GLuint uniformModelView;
static GLuint uniformViewProjection;
static GLuint attribVertexPosition;
static GLuint attribVertexColor;

static GLuint CreateOpenGLShader(char *headerCode, char *vertexCode, char *fragmentCode)
{
    GLint result = GL_FALSE;
    i32 infoLogLength = 0;

    
    // vertex shader
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLchar *vertexShaderCode[] = {
        headerCode,
        vertexCode,
    };
    
    glShaderSource(vertexShaderID, sizeof(vertexShaderCode) / sizeof(vertexShaderCode[0]), vertexShaderCode, 0);
    glCompileShader(vertexShaderID);
    
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0){
        char buffer[4096];
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &buffer[0]);
		printf("error in vertex shader code\n");
		printf(buffer);
        
        Assert(!"failed to compile vertex shader");
    }
    
    
    // fragment shader
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLchar *fragmentShaderCode[] = {
        headerCode,
        fragmentCode,
    };
    glShaderSource(fragmentShaderID, sizeof(fragmentShaderCode) / sizeof(fragmentShaderCode[0]), fragmentShaderCode, 0);
    glCompileShader(fragmentShaderID);
    
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0){
        char buffer[4096];
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &buffer[0]);
		printf("error in fragment shader code\n");
        printf(buffer);
        
        Assert(!"failed to compile fragment shader");
    }
    
    
    // shader
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0){
        char buffer[4096];
        glGetProgramInfoLog(programID, infoLogLength, NULL, &buffer[0]);
		printf("error in shader code\n");
        printf(buffer);
        
        Assert(!"failed to compile fragment shader");
    }
    
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
	
	uniformModelView = glGetUniformLocation(programID, "modelView");
	uniformViewProjection = glGetUniformLocation(programID, "viewProjection");
	attribVertexPosition = glGetAttribLocation(programID, "vertexPosition");
	attribVertexColor = glGetAttribLocation(programID, "vertexColor");
    
    return programID;
}

void InitShader() {
	char *headerCode = (char *)R"AZBD(
	#version 110
	)AZBD";
    
    char *vertexCode = (char *)R"AZBD(
	uniform mat4 modelView;
	uniform mat4 viewProjection;
	
    attribute vec3 vertexPosition;
	attribute vec3 vertexColor;
 
	varying vec3 fragmentColor;
    
    void main() {
		fragmentColor = vertexColor;
		gl_Position = vec4(vertexPosition, 1.0) * modelView * viewProjection;
    }
    
    )AZBD";
    
    char *fragmentCode = (char *)R"AZBD(
    varying vec3 fragmentColor;
    
    void main() {
        gl_FragColor = vec4(fragmentColor, 1.0);
    }
    
    )AZBD";
    debugShader = CreateOpenGLShader(headerCode, vertexCode, fragmentCode);
}

void InitMesh() {
	Vertex vertices[3] = {
		0.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,	0,
		-1.0f, 1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	0,
		1.0f, 1.0f, -1.0f,	 1.0f, 0.0f, 0.0f,	0,
	};
	
	u32 indices[3] = {
		0, 1, 2,
	};
	
	glGenBuffers(1, &vertex_vbo);
    glGenBuffers(1, &index_vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * ArrayCount(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * ArrayCount(indices), indices, GL_STATIC_DRAW);
}

void RenderMesh(m4 modelView, m4 viewProjection) {
	glUseProgram(debugShader);
	
	glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, &modelView.e[0][0]);
	glUniformMatrix4fv(uniformViewProjection, 1, GL_FALSE, &viewProjection.e[0][0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);

	glEnableVertexAttribArray(attribVertexPosition);
	glEnableVertexAttribArray(attribVertexColor);
	
	glVertexAttribPointer(attribVertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(attribVertexColor, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(v3)));
	
	i32 indexCount = 3;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}