
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
	KeyJump,
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

inline bool KeyWasDown(GameInput *gameInput, KeyType keyCode)
{
	InputKey inputKey = gameInput->key[gameInput->keyMap[keyCode]];
	if (inputKey.isDown)
		return 1;
	
	if (inputKey.count >= 2)
		return 1;
	
	return 0;
}

inline bool KeyWasPressed(GameInput *gameInput, KeyType keyCode)
{
	InputKey inputKey = gameInput->key[gameInput->keyMap[keyCode]];
	if (inputKey.isDown && inputKey.count >= 1)
		return 1;
	
	if (inputKey.count >= 2)
		return 1;
	
	return 0;
}

inline bool KeyWasReleased(GameInput *gameInput, KeyType keyCode)
{
	InputKey inputKey = gameInput->key[gameInput->keyMap[keyCode]];
	if (!inputKey.isDown && inputKey.count >= 1)
		return 1;
	
	if (inputKey.count >= 2)
		return 1;
	
	return 0;
}