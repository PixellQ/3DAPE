#include "animator.h"
#include <stdio.h>

Animator::Animator(char* animationName, FbxScene* sceneToBeAnimated, float fps, int totalFrames, struct Landmark* frames)
{
	stackName = animationName;
	scene = sceneToBeAnimated;
	std::cout<<totalFrames;

	totalTime = totalFrames / fps;
	intervalTime = 1 / fps;
}

void Animator::AnimateBones(std::vector<FbxSkeleton*> bonesPresent)
{
	bones = bonesPresent;
	int boneId = 3;
	if (boneId >= 0 && boneId < bones.size())
	{
		FbxSkeleton* skeleton = bones[boneId];
		FbxNode* node = skeleton->GetNode();

		FbxString lAnimStackName = stackName;
		FbxTime lTime;
		int lKeyIndex = 0;
		float rotationvalue = 0.0f;
		FbxNode* lRoot = skeleton->GetNode();
		FbxNode* lLimbNode1 = node->GetChild(0);

		FbxString boneName = node->GetName();
		//printf("Animating bone: %s\n", boneName.Buffer());
		FbxString childName = lLimbNode1->GetName();
		//printf("Animating bone: %s\n", childName.Buffer());

		// First animation stack.
		FbxAnimStack* lAnimStack = FbxAnimStack::Create(scene, lAnimStackName);

		// The animation nodes can only exist on AnimLayers therefore it is mandatory to
		// add at least one AnimLayer to the AnimStack. And for the purpose of this example,
		// one layer is all we need.
		FbxAnimLayer* lAnimLayer = FbxAnimLayer::Create(scene, "Base Layer");
		lAnimStack->AddMember(lAnimLayer);

		// Create the AnimCurve on the Rotation.Y channel
		FbxAnimCurve* lCurve = lRoot->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();
			for (float currenTime = intervalTime; currenTime <= totalTime; currenTime = currenTime + intervalTime)
			{
				lTime.SetSecondDouble(currenTime);
				lKeyIndex = lCurve->KeyAdd(lTime);
				lCurve->KeySetValue(lKeyIndex, rotationvalue);
				lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
				if (rotationvalue <= 45.0)
				{
					rotationvalue = rotationvalue + 0.2f;
				}
			}
		}
		// Same thing for the next object
		lCurve = lLimbNode1->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();
			lTime.SetSecondDouble(0.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 0.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lTime.SetSecondDouble(2.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, -90.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lTime.SetSecondDouble(4.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 90.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lTime.SetSecondDouble(6.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 0.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lCurve->KeyModifyEnd();
		}
	}
}
