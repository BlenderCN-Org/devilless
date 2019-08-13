
#pragma once

#include "platform.h"

struct GameStack
{
	void *base;
	uSize size;

	uSize marker;
};

struct TempMemory
{
	GameStack stack;

	u32 tempCount;
	uSize popMarker[256];
};

struct MemoryInfo
{
	void *base;
	uSize size;
};

inline void StackInit(GameStack *gameStack, MemoryInfo memoryInfo)
{
	gameStack->base = memoryInfo.base;
	gameStack->size = memoryInfo.size;
	gameStack->marker = 0;
}

#define 	StackPushStruct(GameStack, type) (type *)StackPushSize(GameStack, sizeof(type))
#define 	StackPushArray(GameStack, count, type) (type *)StackPushSize(GameStack, (count) * sizeof(type))
inline void * StackPushSize(GameStack *stack, uSize size)
{
	Assert(stack->base != 0);
	Assert((stack->marker + size) <= stack->size);
	void *result = static_cast<u8 *>(stack->base) + stack->marker;
	stack->marker += size;

	return result;
}

inline MemoryInfo StackPushFile(GameStack *stack, char *name)
{
	Assert(stack->base != 0);
	MemoryInfo result = {};
	result.base = (u8 *)(stack->base) + stack->marker;
	result.size = PlatformReadFile(result.base, name);
	
	Assert((stack->marker + result.size) <= stack->size);
	stack->marker += result.size;

	return result;
}

inline void TempMemoryPush(TempMemory *tempMemory)
{
	Assert(tempMemory->stack.base != 0);
	Assert(tempMemory->tempCount < ArrayCount(tempMemory->popMarker));
	tempMemory->popMarker[tempMemory->tempCount] = tempMemory->stack.marker;
	tempMemory->tempCount++;
}

inline void TempMemoryPop(TempMemory *tempMemory)
{
	Assert(tempMemory->stack.base != 0);
	Assert(tempMemory->tempCount > 0);
	tempMemory->tempCount--;
	tempMemory->stack.marker = tempMemory->popMarker[tempMemory->tempCount];
}