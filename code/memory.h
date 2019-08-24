
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

inline void InitStack(GameStack *gameStack, MemoryInfo memoryInfo)
{
	gameStack->base = memoryInfo.base;
	gameStack->size = memoryInfo.size;
	gameStack->marker = 0;
}

#define PushStruct(stack, type) (type *)PushSize(stack, sizeof(type))
#define PushArray(stack, count, type) (type *)PushSize(stack, (count) * sizeof(type))
inline void * PushSize(GameStack *stack, uSize size)
{
	Assert(stack->base != 0);
	Assert((stack->marker + size) <= stack->size);
	void *result = ((u8 *)stack->base) + stack->marker;
	stack->marker += size;

	return result;
}

inline MemoryInfo PushFile(GameStack *stack, char *fileName)
{
	Assert(stack->base != 0);
	MemoryInfo result = {};
	result.base = ((u8 *)stack->base) + stack->marker;
	result.size = ReadFile(result.base, fileName);
	
	Assert((stack->marker + result.size) <= stack->size);
	stack->marker += result.size;

	return result;
}

inline MemoryInfo PushMemoryInfo(GameStack *stack, uSize size) {
	Assert(stack->base != 0);
	Assert((stack->marker + size) <= stack->size);
	
	MemoryInfo result = {};
	result.base = ((u8 *)stack->base) + stack->marker;
	result.size = size;
	stack->marker += size;
	
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