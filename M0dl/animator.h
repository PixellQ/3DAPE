#pragma once

#include <iostream>
#include <vector>
#include <fbxsdk.h>
#include <stdlib.h>


struct Landmark
{
	float x, y, z;

	// Vector subtraction
	Landmark operator-(const Landmark& operand) const {
		return { x - operand.x, y - operand.y, z - operand.z };
	}

	// Scalar division
	Landmark operator/(double scalar) const {
		return { static_cast<float>(x / scalar),
				static_cast<float>(y / scalar),
				static_cast<float>(z / scalar) };
	}

	// Length (magnitude) of the vector
	double length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	// Normalize the vector
	Landmark normalize() const {
		double len = length();
		if (len > 0) {
			return *this / len;
		}
		return { 0.0f, 0.0f, 0.0f }; // Return a zero vector if the length is zero
	}
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
	float totalTime = 0.0f;
	float intervalTime = 0.0f;
	int noOfFrames;
	
	// mediapipe
	std::vector<std::vector<Landmark*>> trackPoints;

	// fbx
	FbxScene* scene;
	std::vector<FbxSkeleton*> bones;

	// Rotation
	Landmark* GetMPOrigin(FbxSkeleton* hipBone);
	Landmark* AlignCordwithOrigin(Landmark* coords, Landmark* Origin);
	//void GetRotation(int boneId);

	void AnimateLeftKnee(FbxScene* fbxScene, int leftHip, int leftKnee, int leftAnkle);

	Landmark CrossProduct(const Landmark& a, const Landmark& b);
	Landmark Normalize(const Landmark& v);
	void MatrixtoEuler(const Landmark& axis_x, const Landmark& axis_y, const Landmark& axis_z,
		double& roll, double& pitch, double& yaw);
	Landmark LookAt(const Landmark& root, const Landmark& target);
};