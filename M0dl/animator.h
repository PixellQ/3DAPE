#pragma once

#include <iostream>
#include <vector>
#include <fbxsdk.h>
#include <stdlib.h>


struct Landmark
{
	float x;
	float y;
	float z;
};


/*Vertex* frame;
Vertex* frames;
*/

class Animator
{
public:

	Animator(char* animationName, FbxScene* sceneToBeAnimated, float fps, int totalFrames, struct Landmark* frames);

	char* stackName;

	void AnimateBones(std::vector<FbxSkeleton*> bonesPresent);

private:

	// Time
	float totalTime;
	float intervalTime;
	
	// fbx
	FbxScene* scene;
	std::vector<FbxSkeleton*> bones;

	// Algo
	//void GetRotation(int boneId);
};