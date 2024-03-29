
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
	//f32 materialID;
	f32 groupIDs[3];
	f32 groupWeights[3];
};

#pragma pack(push, 1)
struct AnimationKey
{
	f32 time;
	m4 transform;
	//v3 position;
	//quat rotation;
};
#pragma pack(pop)

struct Animation
{
	i32 keyCounts[BONE_COUNT];
	AnimationKey *keys[BONE_COUNT];
};

struct Child
{
	i32 childCount;
	i32 childIDs[5];
};

struct Skeleton
{
	m4 inverses[BONE_COUNT];
	
	Child children[BONE_COUNT];
};

enum MeshID {
	MeshGirl,
	
	MESH_ID_COUNT,
};

enum SkinID {
	SkinFemale,
	
	SKIN_ID_COUNT,
};

enum SkeletonID {
	SkeletonFemale,
	
	SKELETON_ID_COUNT,
};

enum AnimationID {
	AnimationFemaleWalk,
	
	ANIMATION_ID_COUNT,
};

enum ShaderID {
	ShaderDebug,
	ShaderSkin,
	
	SHADER_ID_COUNT,
};

void InitMesh(MeshID meshID, char *fileName, TempMemory *tempMemory);
void InitSkin(SkinID skinID, char *fileName, TempMemory *tempMemory);
Skeleton InitSkeleton(char *fileName, TempMemory *tempMemory);
Animation InitAnimation(char *fileName, GameStack *stack);