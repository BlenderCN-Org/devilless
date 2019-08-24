
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
	Player player;
	
	Skeleton skeletons[SKELETON_ID_COUNT];
	Animation animations[ANIMATION_ID_COUNT];
	
	GameStack assetsStack;
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
	f32 deltaTime;
	
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

void GameInit(GameStack *mainStack, TempMemory *tempMemory);
void GameUpdate(GameStack *mainStack, GameInput *gameInput, f32 deltaTime, TempMemory *tempMemory);