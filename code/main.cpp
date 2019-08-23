
#include "renderer.h"
#include "main.h"
#include "game_math.h"
#include "platform.h"

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

void GameInit(GameState *gameState, GameInput *gameInput, TempMemory *tempMemory) {
	InitShader(ShaderDebug);
	InitMesh(MeshGirl, "assets/girl.mesh", tempMemory);
	InitSkin(SkinFemale, "assets/female.skin", tempMemory);
	gameState->skeletons[SkeletonFemale] = InitSkeleton("assets/female.skel", tempMemory);
	
	Assert(tempMemory->tempCount == 0);
}

void GameUpdate(GameState *gameState, GameInput *gameInput, TempMemory *tempMemory) {
	UpdatePlayer(&gameState->player, gameState->deltaTime, gameInput);
	
	v2i screenSize = PlatformGetScreenSize();
	m4 vp = M4(-gameState->player.position) * M4RotY(-gameState->player.yaw) * M4RotX(-gameState->player.pitch) * ProjectionMatrix((f32)screenSize.x / (f32)screenSize.y);
	RenderMesh(MeshGirl, M4Identity(), vp);
	
	Assert(tempMemory->tempCount == 0);
}