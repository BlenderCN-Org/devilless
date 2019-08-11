
#include "renderer.h"

void UpdatePlayer(Player *player, f32 dt, GameInput *gameInput) {
	v2 input = {};
	
	if (KeyWasDown(gameInput, KeyUp))
		input.y -= 1.0f;
	if (KeyWasDown(gameInput, KeyDown))
		input.y += 1.0f;
	if (KeyWasDown(gameInput, KeyLeft))
		input.x -= 1.0f;
	if (KeyWasDown(gameInput, KeyRight))
		input.x += 1.0f;
	
	input = MaxLength1(input);
	
	player->velocity += V3(input.x, 0.0f, input.y) * 54.0f * dt;
	
	player->velocity /= 1.0f + 24.0f;
	
	player->position += player->velocity * dt;
}

void GameInit(GameState *gameState, GameInput *gameInput) {
	/*gameInput->map[KeyUp] = XKeysymToKeycode(display, XK_Up);
	gameInput->map[KeyDown] = XKeysymToKeycode(display, XK_Down);
	gameInput->map[KeyLeft] = XKeysymToKeycode(display, XK_Right);
	gameInput->map[KeyJump] = XKeysymToKeycode(display, XK_Space);
	gameInput->map[KeyPause] = XKeysymToKeycode(display, XK_Escape);*/
	//gameInput->keyMap[KeyPause] = GetScanCode('A');
}

void GameUpdate(GameState *gameState, GameInput *gameInput) {
	UpdatePlayer(&gameState->player, 0.16f, gameInput);
	
	m4 vp = M4(-gameState->player.position) * M4RotY(-gameState->player.yaw) * M4RotX(-gameState->player.pitch) * ProjectionMatrix(540.0f / 480.0f);
	RenderMesh(M4Identity(), vp);
}