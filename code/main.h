
#include "common.h"

struct Player {
	v3 position;
	v3 velocity;
	f32 yaw;
	f32 pitch;
};

struct GameState {
	f32 deltaTime;
	
	Player player;
};

struct InputKey
{
    bool isDown;
    u8 count;
};

enum KeyType
{
	KeyUp,
	KeyDown,
	KeyRight,
	KeyLeft,
	KeyRun,
	KeyJum,
	KeyPause,
	
	KEY_TYPE_COUNT,
};

struct GameInput
{
	v2i mousePos;
    v2i mouseDelta;
	i16 wheelDelta;
	
	u8 keyMap[KEY_TYPE_COUNT];
	InputKey key[256];
};

inline bool KeyWasDown(KeyType keyCode, GameInput *gameInput)
{
	InputKey inputKey = gameInput->key[gameInput->keyMap[keyCode]];
	if (inputKey.isDown)
		return 1;
	
	if (inputKey.count >= 2)
		return 1;
	
	return 0;
}

inline bool KeyWasPressed(KeyType keyCode, GameInput *gameInput)
{
	InputKey inputKey = gameInput->key[gameInput->keyMap[keyCode]];
	if (inputKey.isDown && inputKey.count >= 1)
		return 1;
	
	if (inputKey.count >= 2)
		return 1;
	
	return 0;
}

inline bool KeyWasReleased(KeyType keyCode, GameInput *gameInput)
{
	InputKey inputKey = gameInput->key[gameInput->keyMap[keyCode]];
	if (!inputKey.isDown && inputKey.count >= 1)
		return 1;
	
	if (inputKey.count >= 2)
		return 1;
	
	return 0;
}

void GameInit(GameState *gameState, GameInput *gameInput);
void GameUpdate(GameState *gameState, GameInput *gameInput);
u8 GetKeyCode(u64 keySymbol);