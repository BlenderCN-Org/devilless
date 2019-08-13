
#include "asset_manager.h"
#include "renderer.h"

void InitMesh(MeshID meshID, TempMemory *tempMemory) {
	/*Vertex vertices[3] = {
		0.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,	0,
		-1.0f, 1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	0,
		1.0f, 1.0f, -1.0f,	 1.0f, 0.0f, 0.0f,	0,
	};
	
	u32 indices[3] = {
		0, 1, 2,
	};*/
	
	TempMemoryPush(tempMemory);
	
	MemoryInfo result = StackPushFile(&tempMemory->stack, "assets/girl.mesh");
	
	if (result.size == 0)
		Assert(!"failed to read file");
	
	u8 *tail = (u8 *)result.base;
	
	u32 vertexCount = *((u32 *)tail);
	tail += sizeof(u32);
	u32 indexCount = *((u32 *)tail);
	tail += sizeof(u32);
	
	void *vertices = tail;
	tail += sizeof(Vertex) * vertexCount;
	void *indices = tail;
	tail += sizeof(u16) * indexCount;
	
	InitMeshBuffers(meshID, vertices, vertexCount, indices, indexCount);
}