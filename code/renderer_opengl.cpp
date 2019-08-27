
#include "renderer_opengl.h"
#include "game_math.h"
#include "main.h"

_glGenBuffers *glGenBuffers;
_glBindBuffer *glBindBuffer;
_glBufferData *glBufferData;

_glVertexAttribPointer *glVertexAttribPointer;
_glEnableVertexAttribArray *glEnableVertexAttribArray;
_glGetUniformLocation *glGetUniformLocation;
_glGetAttribLocation *glGetAttribLocation;

_glUniformMatrix4fv *glUniformMatrix4fv;

_glCreateProgram *glCreateProgram;
_glCreateShader *glCreateShader;
_glLinkProgram *glLinkProgram;
_glAttachShader *glAttachShader;
_glShaderSource *glShaderSource;
_glCompileShader *glCompileShader;
_glDeleteShader *glDeleteShader;
_glUseProgram *glUseProgram;

_glGetShaderiv *glGetShaderiv;
_glGetShaderInfoLog *glGetShaderInfoLog;
_glGetProgramiv *glGetProgramiv;
_glGetProgramInfoLog *glGetProgramInfoLog;

RenderState renderState = {};


void CheckOpenGLErrors(GLint shaderID, char *errorMessage) {
	i32 infoLogLength = 0;
	
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0){
        char buffer[4096];
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, &buffer[0]);
		printf(errorMessage);
		printf(buffer);
        
        Assert(!"OpenGL Error, check console for details");
    }
}

void CreateProgram(ShaderInfo *shaderInfo, MemoryInfo vertexShaderMI, MemoryInfo fragmentShaderMI)
{
    // vertex shader
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	
    GLchar *vertexShaderSources[] = { (GLchar *)vertexShaderMI.base };
	GLint vertexSourceLengths[] = { (GLint)vertexShaderMI.size };
    
    glShaderSource(vertexShaderID, 1, vertexShaderSources, vertexSourceLengths);
    glCompileShader(vertexShaderID);
	CheckOpenGLErrors(vertexShaderID, "Error in vertex shader code\n");
    
    // fragment shader
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
    GLchar *fragmentShaderSources[] = { (GLchar *)fragmentShaderMI.base };
	GLint fragmentSourceLengths[] = { (GLint)fragmentShaderMI.size };
	
    glShaderSource(fragmentShaderID, 1, fragmentShaderSources, fragmentSourceLengths);
    glCompileShader(fragmentShaderID);
    CheckOpenGLErrors(fragmentShaderID, "Error in fragment shader code\n");
    
    // shader linking
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
	CheckOpenGLErrors(programID, "error in linking shader\n");
    
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
	
	shaderInfo->programID = programID;
}

void InitShader(ShaderID shaderID, char *vertexFileName, char *fragmentFileName, ShaderDesc shaderDesc, TempMemory *tempMemory) {
	TempMemoryPush(tempMemory);
	
	MemoryInfo vertexShaderMI = PushFile(&tempMemory->stack, vertexFileName);
	MemoryInfo fragmentShaderMI = PushFile(&tempMemory->stack, fragmentFileName);
	
	ShaderInfo *shaderInfo = &renderState.shaderInfos[shaderID];
	
    CreateProgram(shaderInfo, vertexShaderMI, fragmentShaderMI);
	
	shaderInfo->uniformModelView = glGetUniformLocation(shaderInfo->programID, "modelView");
	shaderInfo->uniformViewProjection = glGetUniformLocation(shaderInfo->programID, "viewProjection");
	
	for (i32 i = 0; i < ArrayCount(shaderDesc.uniformNames); i++) {
		if (shaderDesc.uniformNames[i] == 0)
			break;
		
		shaderInfo->uniforms[i] = glGetUniformLocation(shaderInfo->programID, shaderDesc.uniformNames[i]);
	}
	
	for (i32 i = 0; i < ArrayCount(shaderDesc.attribNames); i++) {
		if (shaderDesc.attribNames[i] == 0)
			break;
		
		shaderInfo->attribs[i] = glGetAttribLocation(shaderInfo->programID, shaderDesc.attribNames[i]);
	}
	
	TempMemoryPop(tempMemory);
}

void InitShaders(TempMemory *tempMemory) {
	ShaderDesc shaderDesc = {};
	shaderDesc.attribNames[0] = "vertexPosition";
	shaderDesc.attribNames[1] = "vertexNormal";
	InitShader(ShaderDebug, "assets/shaders/opengl/debug_vertex.glsl", "assets/shaders/opengl/debug_fragment.glsl", shaderDesc, tempMemory);
	
	shaderDesc = {};
	shaderDesc.uniformNames[0] = "materials";
	shaderDesc.uniformNames[1] = "bones";
	
	shaderDesc.attribNames[0] = "vertexPosition";
	shaderDesc.attribNames[1] = "vertexNormal";
	shaderDesc.attribNames[2] = "vertexMaterialID";
	shaderDesc.attribNames[3] = "vertexBoneID";
	shaderDesc.attribNames[4] = "vertexBoneWeight";
	InitShader(ShaderSkin, "assets/shaders/opengl/skin_vertex.glsl", "assets/shaders/opengl/debug_fragment.glsl", shaderDesc, tempMemory);
}

void InternalInitMeshBuffers(MeshInfo *meshInfo, void *vertices, u32 verticesSize, void *indices, u32 indexCount) {
	meshInfo->indexCount = indexCount;
	
	glGenBuffers(1, &meshInfo->vertexVBO);
    glGenBuffers(1, &meshInfo->indexVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, meshInfo->vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshInfo->indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * indexCount, indices, GL_STATIC_DRAW);
}

void InitMeshBuffers(MeshID meshID, void *vertices, u32 verticesSize, void *indices, u32 indexCount) {
	MeshInfo *meshInfo = &renderState.meshInfos[meshID];
	InternalInitMeshBuffers(meshInfo, vertices, verticesSize, indices, indexCount);
}

void InitSkinBuffers(SkinID skinID, void *vertices, u32 verticesSize, void *indices, u32 indexCount) {
	MeshInfo *meshInfo = &renderState.skinInfos[skinID];
	InternalInitMeshBuffers(meshInfo, vertices, verticesSize, indices, indexCount);
}

void RenderMesh(MeshID meshID, m4 modelView, m4 viewProjection) {
	const MeshInfo *meshInfo = &renderState.meshInfos[meshID];
	const ShaderInfo *shaderInfo = &renderState.shaderInfos[ShaderDebug];
	
	glUseProgram(shaderInfo->programID);
	
	glUniformMatrix4fv(shaderInfo->uniformModelView, 1, GL_FALSE, &modelView.e[0][0]);
	glUniformMatrix4fv(shaderInfo->uniformViewProjection, 1, GL_FALSE, &viewProjection.e[0][0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, meshInfo->vertexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshInfo->indexVBO);

	glEnableVertexAttribArray(shaderInfo->attribs[0]);
	glEnableVertexAttribArray(shaderInfo->attribs[1]);
	
	glVertexAttribPointer(shaderInfo->attribs[0], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(shaderInfo->attribs[1], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(v3)));
	
	glDrawElements(GL_TRIANGLES, meshInfo->indexCount, GL_UNSIGNED_SHORT, 0);
}

void RenderSkin(SkinID skinID, m4 modelView, m4 viewProjection) {
	const MeshInfo *meshInfo = &renderState.skinInfos[skinID];
	const ShaderInfo *shaderInfo = &renderState.shaderInfos[ShaderSkin];
	
	glUseProgram(shaderInfo->programID);
	
	glUniformMatrix4fv(shaderInfo->uniformModelView, 1, GL_FALSE, &modelView.e[0][0]);
	glUniformMatrix4fv(shaderInfo->uniformViewProjection, 1, GL_FALSE, &viewProjection.e[0][0]);
	
	m4 bones[BONE_COUNT] = {};
	
	for (i32 i = 0; i < BONE_COUNT; i++) {
		bones[i] = M4Identity();
	}
	bones[0] = M4(20, 0, 0);
	
	glUniformMatrix4fv(shaderInfo->uniforms[1], BONE_COUNT, GL_FALSE, &bones[0].e[0][0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, meshInfo->vertexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshInfo->indexVBO);

	glEnableVertexAttribArray(shaderInfo->attribs[0]);
	glEnableVertexAttribArray(shaderInfo->attribs[1]);
	
	u8 offset = 0;
	glVertexAttribPointer(shaderInfo->attribs[0], 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), 0);
	offset += sizeof(v3);
	glVertexAttribPointer(shaderInfo->attribs[1], 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void *)offset);
	offset += sizeof(v3);
	glVertexAttribPointer(shaderInfo->attribs[3], 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void *)offset);
	offset += sizeof(v3);
	glVertexAttribPointer(shaderInfo->attribs[4], 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void *)offset);
	
	glDrawElements(GL_TRIANGLES, meshInfo->indexCount, GL_UNSIGNED_SHORT, 0);
}

void ClearFrame() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	v2i screenSize = GetScreenSize();
	glViewport(0, 0, screenSize.x, screenSize.y);
}