
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
	gameInput->keyMap[KeyUp] = PlatformGetKeyCode('W');
	gameInput->keyMap[KeyDown] = PlatformGetKeyCode('S');
	gameInput->keyMap[KeyLeft] = PlatformGetKeyCode('A');
	gameInput->keyMap[KeyRight] = PlatformGetKeyCode('D');
	gameInput->keyMap[KeyRun] = PlatformGetKeyCode('C');
	gameInput->keyMap[KeyPause] = PlatformGetKeyCode('E');
	
	InitShader(ShaderDebug);
	InitMesh(MeshTriangle, tempMemory);
}

void GameUpdate(GameState *gameState, GameInput *gameInput, TempMemory *tempMemory) {
	UpdatePlayer(&gameState->player, gameState->deltaTime, gameInput);
	
	m4 vp = M4(-gameState->player.position) * M4RotY(-gameState->player.yaw) * M4RotX(-gameState->player.pitch) * ProjectionMatrix(540.0f / 480.0f);
	RenderMesh(MeshTriangle, M4Identity(), vp);
}