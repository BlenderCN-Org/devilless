
void GameInit(GameInput *gameInput) {
	/*gameInput->map[KeyUp] = XKeysymToKeycode(display, XK_Up);
	gameInput->map[KeyDown] = XKeysymToKeycode(display, XK_Down);
	gameInput->map[KeyLeft] = XKeysymToKeycode(display, XK_Right);
	gameInput->map[KeyJump] = XKeysymToKeycode(display, XK_Space);
	gameInput->map[KeyPause] = XKeysymToKeycode(display, XK_Escape);*/
	//gameInput->keyMap[KeyPause] = GetScanCode('A');
	
}

void GameUpdate(GameInput *gameInput) {
	if (KeyWasPressed(gameInput, KeyPause))
		printf("0 was pressed\n");
}