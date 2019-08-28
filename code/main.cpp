
#include "renderer.h"
#include "main.h"
#include "game_math.h"
#include "platform.h"
#include "animation.h"

void UpdatePlayer(Player *player, f32 dt, GameInput *gameInput) {
	v2 input = {};
	
	if (KeyWasDown(KeyUp, gameInput))
		input.y -= 1.0f;
	if (KeyWasDown(KeyDown, gameInput))
		input.y += 1.0f;
	if (KeyWasDown(KeyLeft, gameInput))
		input.x -= 1.0f;
	if (KeyWasDown(KeyRight, gameInput))
		input.x += 1.0f;
	
	input = MaxLength1(input);
	
	f32 speed = 100.0f + KeyWasDown(KeyRun, gameInput) ? 35.0f : 0.0f;
	player->velocity += V3(input.x, 0.0f, input.y) * speed * dt;
	
	player->velocity /= 1.0f + 23.0f * dt;
	
	player->position += player->velocity * dt;
}

void GameInit(GameStack *mainStack, TempMemory *tempMemory) {
	GameState *gameState = PushStruct(mainStack, GameState);
	
	InitStack(&gameState->assetsStack, PushMemoryInfo(mainStack, Megabytes(200)));
	
	InitMesh(MeshGirl, "assets/girl.mesh", tempMemory);
	InitSkin(SkinFemale, "assets/hoodie.skin", tempMemory);
	gameState->skeletons[SkeletonFemale] = InitSkeleton("assets/player.skel", tempMemory);
	gameState->animations[AnimationFemaleWalk] = InitAnimation("assets/idle.anim", &gameState->assetsStack);
	
	gameState->player.position = V3(0.0f, 1.8f, 0.0f);
	
	Assert(tempMemory->tempCount == 0);
}

void GameUpdate(GameStack *mainStack, GameInput *gameInput, f32 deltaTime, TempMemory *tempMemory) {
	GameState *gameState = (GameState *)mainStack->base;
	UpdatePlayer(&gameState->player, deltaTime, gameInput);
	
	v2i screenSize = GetScreenSize();
	m4 vp = M4(-gameState->player.position) * M4RotY(-gameState->player.yaw) * M4RotX(-gameState->player.pitch) * ProjectionMatrix((f32)screenSize.x / (f32)screenSize.y);
	
	MeshBuffer meshBuffer = { M4Identity(), vp };
	RenderMesh(MeshGirl, &meshBuffer);
	
	SkinBuffer skinBuffer = {};
	skinBuffer.modelView = M4(1.0f, 0, 0);
	skinBuffer.viewProjection = vp;
	GetBones(&gameState->animations[AnimationFemaleWalk], &gameState->skeletons[SkeletonFemale], skinBuffer.bones);
	
	RenderSkin(SkinFemale, &skinBuffer);
	
	Assert(tempMemory->tempCount == 0);
}