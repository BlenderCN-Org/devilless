
#include "renderer.h"
#include "main.h"

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

void GameInit(GameState *gameState, GameInput *gameInput) {
	gameInput->keyMap[KeyUp] = GetKeyCode('W');
	gameInput->keyMap[KeyDown] = GetKeyCode('S');
	gameInput->keyMap[KeyLeft] = GetKeyCode('A');
	gameInput->keyMap[KeyRight] = GetKeyCode('D');
	gameInput->keyMap[KeyRun] = GetKeyCode('C');
	gameInput->keyMap[KeyPause] = GetKeyCode('E');
	
	InitShader();
	InitMesh();
}

void GameUpdate(GameState *gameState, GameInput *gameInput) {
	UpdatePlayer(&gameState->player, gameState->deltaTime, gameInput);
	
	m4 vp = M4(-gameState->player.position) * M4RotY(-gameState->player.yaw) * M4RotX(-gameState->player.pitch) * ProjectionMatrix(540.0f / 480.0f);
	RenderMesh(M4Identity(), vp);
}