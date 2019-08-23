
#include "common.h"
#include "memory.h"
#include "asset_manager.h"

struct Player {
	v3 position;
	v3 velocity;
	f32 yaw;
	f32 pitch;
};

struct GameState {
	f32 deltaTime;
	
	Player player;
	
	Skeleton skeletons[SKELETON_ID_COUNT];
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
	InputKey keys[256];
};

inline bool KeyWasDown(KeyType keyCode, GameInput *gameInput)
{
	InputKey inputKey = gameInput->keys[gameInput->keyMap[keyCode]];
	if (inputKey.isDown)
		return 1;
	
	if (inputKey.count >= 2)
		return 1;
	
	return 0;
}

inline bool KeyWasPressed(KeyType keyCode, GameInput *gameInput)
{
	InputKey inputKey = gameInput->keys[gameInput->keyMap[keyCode]];
	if (inputKey.isDown && inputKey.count >= 1)
		return 1;
	
	if (inputKey.count >= 2)
		return 1;
	
	return 0;
}

inline bool KeyWasReleased(KeyType keyCode, GameInput *gameInput)
{
	InputKey inputKey = gameInput->keys[gameInput->keyMap[keyCode]];
	if (!inputKey.isDown && inputKey.count >= 1)
		return 1;
	
	if (inputKey.count >= 2)
		return 1;
	
	return 0;
}

void GameInit(GameState *gameState, GameInput *gameInput, TempMemory *tempMemory);
void GameUpdate(GameState *gameState, GameInput *gameInput, TempMemory *tempMemory);