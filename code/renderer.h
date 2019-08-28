
#include "common.h"
#include "asset_manager.h"
#include "memory.h"

struct MeshBuffer;
struct SkinBuffer;

bool InitRenderer(TempMemory *tempMemory);

void InitMeshBuffers(MeshID meshID, void *vertices, u32 verticesSize, void *indices, u32 indicesSize);
void InitSkinBuffers(SkinID skinID, void *vertices, u32 verticesSize, void *indices, u32 indicesSize);

void RenderMesh(MeshID meshID, MeshBuffer *meshBuffer);
void RenderSkin(SkinID skinID, SkinBuffer *skinBuffer);


void ClearFrame();
void PresentFrame();

struct MeshBuffer {
	m4 modelView;
	m4 viewProjection;
};

struct SkinBuffer {
	m4 modelView;
	m4 viewProjection;
	
	m4 bones[BONE_COUNT];
};