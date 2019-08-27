
#include "common.h"
#include "asset_manager.h"
#include "memory.h"

bool InitRenderer(TempMemory *tempMemory);
void RenderMesh(MeshID meshID, m4 modelView, m4 viewProjection);
void RenderSkin(SkinID skinID, m4 modelView, m4 viewProjection);
void InitMeshBuffers(MeshID meshID, void *vertices, u32 verticesSize, void *indices, u32 indicesSize);
void InitSkinBuffers(SkinID skinID, void *vertices, u32 verticesSize, void *indices, u32 indicesSize);

void ClearFrame();
void PresentFrame();