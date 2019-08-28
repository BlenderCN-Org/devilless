
#include "animation.h"
#include "game_math.h"

void GetBonesRecursive(Animation *animation, Skeleton *skeleton, i32 id, m4 accumTransform, m4 *bones) {
	accumTransform = animation->keys[id]->transform * accumTransform;
	bones[id] = skeleton->inverses[id] * accumTransform;
	
	for (i32 i = 0; i < skeleton->children[id].childCount; i++)
	{
		GetBonesRecursive(animation, skeleton, skeleton->children[id].childIDs[i], accumTransform, bones);
	}
}

void GetBones(Animation *animation, Skeleton *skeleton, m4 *bones) {
	GetBonesRecursive(animation, skeleton, 0, M4Identity(), bones);
}
