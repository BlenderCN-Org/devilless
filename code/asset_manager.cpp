
#include "asset_manager.h"
#include "renderer.h"

void InitMesh(MeshID meshID, char *fileName, TempMemory *tempMemory) {
	TempMemoryPush(tempMemory);
	
	MemoryInfo result = StackPushFile(&tempMemory->stack, fileName);
	
	Assert(result.size != 0);
	
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
	
	TempMemoryPop(tempMemory);
}

void InitSkin(SkinID skinID, char *fileName, TempMemory *tempMemory) {
	TempMemoryPush(tempMemory);
	
	MemoryInfo result = StackPushFile(&tempMemory->stack, fileName);
	
	Assert(result.size != 0);
	
	u8 *tail = (u8 *)result.base;
	
	u32 vertexCount = *((u32 *)tail);
	tail += sizeof(u32);
	u32 indexCount = *((u32 *)tail);
	tail += sizeof(u32);
	
	void *vertices = tail;
	tail += sizeof(SkinVertex) * vertexCount;
	void *indices = tail;
	tail += sizeof(u16) * indexCount;
	
	InitSkinBuffers(skinID, vertices, vertexCount, indices, indexCount);
	
	TempMemoryPop(tempMemory);
}