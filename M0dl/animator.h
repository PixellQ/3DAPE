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

	Animator(const char* animationName, FbxScene* sceneToBeAnimated, float fps, int totalFrames, struct Landmark* frames);

	const char* stackName;

	void AnimateBones(std::vector<FbxSkeleton*> bonesPresent);

private:

	// Time
	float totalTime;
	float intervalTime;
	
	// mediapipe
	std::vector<std::vector<Landmark*>> trackPoints;

	// fbx
	FbxScene* scene;
	std::vector<FbxSkeleton*> bones;

	// Algo
	Landmark* GetMPOrigin(FbxSkeleton* hipBone);
	Landmark* AlignCordwithOrigin(Landmark* coords, Landmark* Origin);
	//void GetRotation(int boneId);
};