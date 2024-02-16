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

	// Functions
	Landmark operator-(const Landmark& operand) const;
	Landmark operator/(double scalar) const;
	double length() const;
};


struct Quaternion
{
	double w, x, y, z;
};



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

	// Rotation
	Landmark* GetMPOrigin(FbxSkeleton* hipBone);
	Landmark* AlignCordwithOrigin(Landmark* coords, Landmark* Origin);
	//void GetRotation(int boneId);

	Landmark CrossProduct(const Landmark& a, const Landmark& b);
	Landmark Normalize(const Landmark& v);
	void MatrixtoEuler(const Landmark& axis_x, const Landmark& axis_y, const Landmark& axis_z,
		double& roll, double& pitch, double& yaw);
	Landmark LookAt(const Landmark& base, const Landmark& child);
};