
#include "asset_manager.h"
#include "renderer.h"

void InitMesh(MeshID meshID, TempMemory *tempMemory) {
	Vertex vertices[3] = {
		0.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,	0,
		-1.0f, 1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	0,
		1.0f, 1.0f, -1.0f,	 1.0f, 0.0f, 0.0f,	0,
	};
	
	u32 indices[3] = {
		0, 1, 2,
	};
	
	InitMeshBuffers(meshID, vertices, sizeof(vertices), indices, sizeof(indices));
}