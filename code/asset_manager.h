
#pragma once

#include "common.h"
#include "memory.h"


// TODO: do we really need these vertex structs?
// we could/should put the vertex stride in the file.
struct Vertex {
	v3 position;
	v3 normal;
	u32 matID;
};

struct SkinVertex {
	v3 position;
	v3 normal;
	i32 groupId[3];
	f32 groupWeight[3];
};

enum MeshID {
	MeshGirl,
	
	MESH_ID_COUNT,
};

enum SkinID {
	SkinFemale,
	
	SKIN_ID_COUNT,
};

enum ShaderID {
	ShaderDebug,
	
	SHADER_ID_COUNT,
};

void InitMesh(MeshID meshID, char *fileName, TempMemory *tempMemory);
void InitSkin(SkinID skinID, char *fileName, TempMemory *tempMemory);