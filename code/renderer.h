
#include "common.h"
#include "asset_manager.h"

bool InitRenderer();
void RenderMesh(MeshID meshID, m4 modelView, m4 viewProjection);
void InitShader(ShaderID shaderID);
void InitMeshBuffers(MeshID meshID, void *vertices, u32 verticesSize, void *indices, u32 indicesSize);

void ClearFrame();
void PresentFrame();