#pragma once

struct GameInput;

uSize ReadFile(void *base, char *fileName);
v2i GetScreenSize();
void InitInput(GameInput *gameInput);