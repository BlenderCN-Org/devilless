
#include "asset_manager.h"
#include "renderer.h"


// TODO: cleanup casts and pointer movement with macros
void InitMesh(MeshID meshID, char *fileName, TempMemory *tempMemory) {
	TempMemoryPush(tempMemory);
	
	MemoryInfo result = PushFile(&tempMemory->stack, fileName);
	
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
	
	InitMeshBuffers(meshID, vertices, vertexCount * sizeof(Vertex), indices, indexCount);
	
	TempMemoryPop(tempMemory);
}

void InitSkin(SkinID skinID, char *fileName, TempMemory *tempMemory) {
	TempMemoryPush(tempMemory);
	
	MemoryInfo result = PushFile(&tempMemory->stack, fileName);
	
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
	
	InitSkinBuffers(skinID, vertices, vertexCount * sizeof(SkinVertex), indices, indexCount);
	
	TempMemoryPop(tempMemory);
}

Skeleton InitSkeleton(char *fileName, TempMemory *tempMemory) {
	TempMemoryPush(tempMemory);
	
	MemoryInfo result = PushFile(&tempMemory->stack, fileName);
	Assert(result.size != 0);
	
	Skeleton skeleton = {};
	
	u8 *tail = (u8 *)result.base;
	u32 boneCount = *((u32 *)tail);
	tail += sizeof(u32);
	
	Assert(boneCount == BONE_COUNT);
	
	for (i32 i = 0; i < BONE_COUNT; i++) {
		skeleton.inverses[i] = *((m4 *)tail);
		tail += sizeof(m4);
		
		skeleton.children[i].childCount = *((i32 *)tail);
		tail += sizeof(i32);
		
		Assert(skeleton.children[i].childCount <= 5);
		
		for (i32 j = 0; j < skeleton.children[i].childCount; j++) {
			skeleton.children[i].childIDs[j] = *((i32 *)tail);
			tail += sizeof(i32);
		}
	}
	
	TempMemoryPop(tempMemory);
	
	return skeleton;
}

Animation InitAnimation(char *fileName, GameStack *stack) {
	MemoryInfo result = PushFile(stack, fileName);
	Assert(result.size != 0);
	
	Animation animation = {};
	
	u8 *tail = (u8 *)result.base;
	u32 boneCount = *((u32 *)tail);
	tail += sizeof(u32);
	
	Assert(boneCount == BONE_COUNT);
	
	for (i32 i = 0; i < BONE_COUNT; i++) {
		animation.keys[i] = ((AnimationKey *)tail);
		tail += sizeof(AnimationKey);
	}
	
	return animation;
}