
#pragma once

#include "common.h"
#include "memory.h"

struct Vertex {
	v3 position;
	v3 normal;
	u32 matID;
};

enum MeshID {
	MeshTriangle,
	
	MESH_ID_COUNT,
};

enum ShaderID {
	ShaderDebug,
	
	SHADER_ID_COUNT,
};

void InitMesh(MeshID meshID, TempMemory *tempMemory);