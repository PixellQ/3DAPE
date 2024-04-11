#include "animator.h"
#include <stdio.h>


Animator::Animator(const char* animationName, FbxScene* sceneToBeAnimated, float fps, int totalFrames, struct Landmark* frames)
{
	stackName = animationName;
	scene = sceneToBeAnimated;
	noOfFrames = totalFrames;
	std::cout << "Total Frames : " << totalFrames << std::endl;

	for (int i = 0; i < 33; ++i) {
		std::vector<Landmark*> trackedPoint;
		for (int j = 0; j <= totalFrames; j++)
		{
			trackedPoint.push_back(&frames[i + (j * 33)]);
		}
		trackedPoint.insert(trackedPoint.begin(), &frames[i]);

		trackPoints.push_back(trackedPoint);
	}

	for (int i = 0; i <= totalFrames; i++)
	{
		Landmark rot = LookAt(*trackPoints[12][i], *trackPoints[24][i]);

		std::cout << std::endl << "Frame no. : " << i;
		std::cout << std::endl << "Roll: " << rot.x;
		std::cout << " Pitch: " << rot.y;
		std::cout << " Yaw: " << rot.z;
	}

	totalTime = totalFrames / 30.0f;
	intervalTime = 1.0f / 30.0f;

}

void Animator::AnimateBones(std::vector<FbxSkeleton*> bonesPresent)
{
	//double roll, pitch, yaw;
	bones = bonesPresent;
	int boneId = 50;
	if (boneId >= 0 && boneId < bones.size())
	{
		FbxSkeleton* skeleton = bones[boneId];
		FbxNode* node = skeleton->GetNode();

		FbxString lAnimStackName = stackName;
		FbxTime lTime;
		int lKeyIndex = 0;
		FbxNode* lRoot = skeleton->GetNode();
		FbxNode* lLimbNode1 = node->GetChild(0);

		FbxAnimStack* lAnimStack = FbxAnimStack::Create(scene, lAnimStackName);
		FbxAnimLayer* lAnimLayer = FbxAnimLayer::Create(scene, "Base Layer");
		lAnimStack->AddMember(lAnimLayer);

		int i = 0;
		float rotationvalue = 0.0f;
		FbxAnimCurve* lCurve = lRoot->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
		/*if (lCurve)
		{
			
			lCurve->KeyModifyBegin();
			for (float currenTime = 0.0f; currenTime <= totalTime - intervalTime; currenTime += intervalTime)
			{
				lTime.SetSecondDouble(currenTime);
				lKeyIndex = lCurve->KeyAdd(lTime);
				lCurve->KeySetValue(lKeyIndex, rotationvalue);
				lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);

				Landmark origin;
				origin.x = 0;
				origin.y = 0;
				origin.z = 0;
				rotationvalue = rotationvalue + LookAt(*trackPoints[12][i], *trackPoints[14][i]).x;
				i += 1;
			}
			lCurve->KeyModifyEnd();
		}*/
		i = 0;
		rotationvalue = 0.0f;
		lCurve = lRoot->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();
			for (float currenTime = 0.0f; currenTime <= totalTime - intervalTime; currenTime = currenTime + intervalTime)
			{
				lTime.SetSecondDouble(currenTime);
				lKeyIndex = lCurve->KeyAdd(lTime);
				lCurve->KeySetValue(lKeyIndex, rotationvalue);
				lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);

				rotationvalue = rotationvalue + LookAt(*trackPoints[12][i], *trackPoints[14][i]).z;
				i += 1;
			}
			lCurve->KeyModifyEnd();
		}
		i = 0;
		rotationvalue = 0.0f;
		lCurve = lRoot->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();
			for (float currenTime = 0.0f; currenTime <= totalTime - intervalTime; currenTime = currenTime + intervalTime)
			{
				lTime.SetSecondDouble(currenTime);
				lKeyIndex = lCurve->KeyAdd(lTime);
				lCurve->KeySetValue(lKeyIndex, rotationvalue);
				lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);

				rotationvalue = rotationvalue + LookAt(*trackPoints[12][i], *trackPoints[14][i]).y;
				i += 1;
			}
			lCurve->KeyModifyEnd();
		}
	}
}

Landmark* Animator::GetMPOrigin(FbxSkeleton* hipBone)
{
	FbxSkeleton* skeleton = hipBone;
	FbxNode* node = skeleton->GetNode();
	FbxString boneName = node->GetName();

	FbxAMatrix globalTransform = node->EvaluateGlobalTransform();

	FbxVector4 translation = globalTransform.GetT();
	FbxVector4 rotation = globalTransform.GetR();

	Landmark* Origin;
	Origin->x = translation[0];
	Origin->y = translation[1];
	Origin->z = translation[2];

	return Origin;
}

Landmark* Animator::AlignCordwithOrigin(Landmark* coords, Landmark* Origin)
{
	Landmark* newLandmark;
	newLandmark->x = coords->x + Origin->x;
	newLandmark->y = coords->y + Origin->y;
	newLandmark->z = coords->z + Origin->z;

	return newLandmark;
}

Landmark Animator::CrossProduct(const Landmark& a, const Landmark& b)
{
	Landmark result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;

	return result;
}

Landmark Animator::Normalize(const Landmark& v)
{
	double norm = v.length();
	if (norm == 0) {
		return v;
	}
	return v / norm;
}

void Animator::MatrixtoEuler(const Landmark& axis_x, const Landmark& axis_y, const Landmark& axis_z, double& roll, double& pitch, double& yaw)
{
	// Calculate pitch
	pitch = std::asin(-axis_z.x);

	// Check for singularity at the poles
	if (std::cos(pitch) != 0) {
		// Calculate roll and yaw only if not at the poles
		roll = std::atan2(axis_z.y, axis_z.z);
		yaw = std::atan2(axis_y.x, axis_x.x);
	}
	else {
		// At the poles, set roll to zero and calculate yaw
		roll = 0.0;
		yaw = std::atan2(-axis_x.y, axis_y.y);
	}
}

Landmark Animator::LookAt(const Landmark& root, const Landmark& target)
{
	Landmark axis_z = Normalize(root - target);
	if (axis_z.length() == 0) {

		axis_z.x = 0;
		axis_z.y = -1;
		axis_z.z = 0;
	}

	Landmark tmp;
	tmp.x = 0;
	tmp.y = 0;
	tmp.z = 1;

	Landmark axis_x = CrossProduct(tmp, axis_z);
	if (axis_x.length() == 0) {

		axis_x.x = 1;
		axis_x.y = 0;
		axis_x.z = 0;
	}

	Landmark axis_y = CrossProduct(axis_z, axis_x);

	// Assuming a 3x3 rotation matrix as in the original code
	double rot_matrix[3][3] = { {axis_x.x, axis_y.x, axis_z.x},
							   {axis_x.y, axis_y.y, axis_z.y},
							   {axis_x.z, axis_y.z, axis_z.z} };

	double roll, pitch, yaw;


	MatrixtoEuler(axis_x, axis_y, axis_z, roll, pitch, yaw);

	Landmark result;
	result.x = roll;
	result.y = pitch;
	result.z = yaw;

	return result;
}

Landmark Landmark::operator-(const Landmark& operand) const
{
	Landmark result;
	result.x = x - operand.x;
	result.y = y - operand.y;
	result.z = z - operand.z;

	return result;
}

Landmark Landmark::operator/(double scalar) const
{
	Landmark result;
	result.x = x / scalar;
	result.y = y / scalar;
	result.z = z / scalar;

	return result;
}

double Landmark::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}