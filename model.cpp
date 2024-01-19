#include <iostream>
#include <vector>
#include <fbxsdk.h>
#include <stdlib.h>

#define DLLEXPORT extern "C" __declspec(dllexport)

std::vector<FbxMesh*> meshes;
std::vector<FbxSkeleton*> bones;

FbxManager* manager;
FbxScene* scene;

struct Vertex 
{
	float x;
	float y;
	float z;
};

struct Polygon
{
	int polygonSize;
	int index;
}; 

struct Mesh 
{
    int polygonCount;
    Polygon* polygons;

	int vertexCount;
	Vertex* vertices;

	int indexCount;
	int* indices;

	int normalCount;
	Vertex* normals;
};

struct Bone
{
    int boneId;
    char* boneName;
};

struct Scene
{
    int meshCount;
	Mesh* meshes;

    int boneCount;
    Bone* bones;
};

Scene* custom_scene;

void StoreDetails(FbxNode* parent) 
{
	FbxMesh* mesh = parent->GetMesh();
	if (mesh != NULL)
		meshes.push_back(mesh);
	else{
		FbxSkeleton* skeleton = parent->GetSkeleton();
		if (skeleton != NULL)
			bones.push_back(skeleton);
	}
	for (int i = 0; i < parent->GetChildCount(); i++) {
		StoreDetails(parent->GetChild(i));
	}
}


DLLEXPORT Scene* GetSceneDetails() 
{
	Mesh* tmp_mesh = (Mesh*)malloc(sizeof(Mesh) * meshes.size());
	Bone* tmp_bone = (Bone*)malloc(sizeof(Bone) * bones.size());

	Scene* sceneDetails = (Scene*)malloc(sizeof(Scene));

	for (int i = 0; i < meshes.size(); i++)
	{
		int numOfPolygon = meshes[i]->GetPolygonCount();
		Polygon* tmp_polygon = (Polygon*)malloc(sizeof(Polygon) * numOfPolygon);

		for (int j = 0; j < numOfPolygon; j++)
		{
			tmp_polygon[j].polygonSize = meshes[i]->GetPolygonSize(j);
			tmp_polygon[j].index = meshes[i]->GetPolygonVertexIndex(j);
		}
		tmp_mesh[i].polygonCount = numOfPolygon;
		tmp_mesh[i].polygons = tmp_polygon;
		
		int numOfControlPoints = meshes[i]->GetControlPointsCount();
		Vertex* vertices = (Vertex*)malloc(sizeof(Vertex) * numOfControlPoints);
		int numOfIndices = meshes[i]->GetPolygonVertexCount();
		int* indices = (int*)malloc(sizeof(int) * numOfIndices);

		FbxVector4* controlpoints = meshes[i]->GetControlPoints();
		for (int j = 0; j < numOfControlPoints; j++)
		{
			FbxVector4 transformation = controlpoints[j];
			vertices[j].x = transformation.mData[0];
			vertices[j].y = transformation.mData[1];
			vertices[j].z = transformation.mData[2];
		}
		//int* index = meshes[i]->GetPolygonVertices();

		//for (size_t j = 0; j < numOfIndices; j++) indices[j] = index[j];

		tmp_mesh[i].vertexCount = numOfControlPoints;
		tmp_mesh[i].vertices = vertices;
		tmp_mesh[i].indexCount = numOfIndices;
		tmp_mesh[i].indices = meshes[i]->GetPolygonVertices();;

		FbxArray<FbxVector4> normalsarray;
		meshes[i]->GetPolygonVertexNormals(normalsarray);
		Vertex* normals = (Vertex*)malloc(sizeof(Vertex) * normalsarray.Size());

		for (int j = 0; j < normalsarray.Size(); j++)
		{
			FbxVector4 transformation = normalsarray[j];
			normals[j].x = transformation.mData[0];
			normals[j].y = transformation.mData[1];
			normals[j].z = transformation.mData[2];
		}
		tmp_mesh[i].normalCount = normalsarray.Size();
		tmp_mesh[i].normals = normals;
	}

	for (int i = 0; i < bones.size(); i++)
	{
		tmp_bone[i].boneId = i;
		FbxNode* node = bones[i]->GetNode();
		if (node != nullptr)
		{
			const char* boneName = node->GetName();
			int nameLength = strlen(boneName);
			tmp_bone[i].boneName = (char*)malloc(sizeof(char) * (nameLength + 1));
			strcpy_s(tmp_bone[i].boneName, nameLength + 1, boneName);
		}
		else
		{
			tmp_bone[i].boneName = nullptr;
		}
		//memcpy(bone[i].name, bones[i]->GetName(), sizeof(bones[i]->GetName())/sizeof(char));
	}
	sceneDetails->meshCount = meshes.size();
	sceneDetails->meshes = tmp_mesh;
	sceneDetails->boneCount = bones.size();
	sceneDetails->bones = tmp_bone;

	custom_scene = sceneDetails;
	return sceneDetails;
}


DLLEXPORT int OpenFile(char* filename)
{
	manager = FbxManager::Create();
	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename, -1, manager->GetIOSettings());

	scene = FbxScene::Create(manager, "");
	importer->Import(scene);
	importer->Destroy();

	StoreDetails(scene->GetRootNode());

	if (!(bones.size() > 0))
	{
		return 1;
	}
	return 0;
}


/*DLLEXPORT void AnimateRotation(int boneId, float rotationAmount)
{
	if (boneId >= 0 && boneId < bones.size())
	{
		FbxSkeleton* skeleton = bones[boneId];
		FbxNode* node = skeleton->GetNode();

		// Create an animation stack and layer if they don't exist
		FbxAnimStack* animStack = FbxAnimStack::Create(scene, "AnimationStack");
		FbxAnimLayer* animLayer = FbxAnimLayer::Create(scene, "AnimationLayer");
		animStack->AddMember(animLayer);

		// Access the rotation property
		FbxProperty rotationProperty = node->LclRotation;

		// Access the curves for each axis (X, Y, Z)
		FbxAnimCurve* curveX = rotationProperty.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
		FbxAnimCurve* curveY = rotationProperty.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
		FbxAnimCurve* curveZ = rotationProperty.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);

		// Set the rotation keyframes
		FbxTime startTime = FbxTime(0);
		FbxTime endTime = FbxTime(1);

		curveX->KeyModifyBegin();
		curveY->KeyModifyBegin();
		curveZ->KeyModifyBegin();

		curveX->KeyAdd(startTime, rotationAmount);
		curveY->KeyAdd(startTime, rotationAmount);
		curveZ->KeyAdd(startTime, rotationAmount);

		curveX->KeyAdd(endTime, rotationAmount);
		curveY->KeyAdd(endTime, rotationAmount);
		curveZ->KeyAdd(endTime, rotationAmount);

		curveX->KeyModifyEnd();
		curveY->KeyModifyEnd();
		curveZ->KeyModifyEnd();

		// Update the animation stack
		scene->AddTake(animStack);
		scene->SetCurrentAnimationStack(animStack);
		scene->SetCurrentAnimationLayer(animLayer);
		scene->Evaluate();
	}
}
*/
DLLEXPORT void AnimateRotation(int boneId)//, float rotationAmount)
{
	if (boneId >= 0 && boneId < bones.size())
	{
		FbxSkeleton* skeleton = bones[boneId];
		FbxNode* node = skeleton->GetNode();

		FbxString lAnimStackName;
		FbxTime lTime;
		int lKeyIndex = 0;
		FbxNode* lRoot = skeleton->GetNode();
		FbxNode* lLimbNode1 = node->GetChild(0);

		FbxString boneName = node->GetName();
		printf("Animating bone: %s\n", boneName.Buffer());
		FbxString childName = lLimbNode1->GetName();
		printf("Animating bone: %s\n", childName.Buffer());

		// First animation stack.
		lAnimStackName = "Bend on 2 sides";
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
			lTime.SetSecondDouble(0.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 0.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lTime.SetSecondDouble(2.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 45.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lTime.SetSecondDouble(4.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, -45.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lTime.SetSecondDouble(5.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 0.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lCurve->KeyModifyEnd();
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
		// Second animation stack.
		lAnimStackName = "Bend and turn around";
		lAnimStack = FbxAnimStack::Create(scene, lAnimStackName);
			// The animation nodes can only exist on AnimLayers therefore it is mandatory to
			// add at least one AnimLayer to the AnimStack. And for the purpose of this example,
			// one layer is all we need.
			lAnimLayer = FbxAnimLayer::Create(scene, "Base Layer");
			lAnimStack->AddMember(lAnimLayer);
		// Create the AnimCurve on the Rotation.X channel
		lCurve = lRoot->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();
			lTime.SetSecondDouble(0.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 0.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lTime.SetSecondDouble(2.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 720.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lCurve->KeyModifyEnd();
		}
		lCurve = lLimbNode1->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
		if (lCurve)
		{
			lCurve->KeyModifyBegin();
			lTime.SetSecondDouble(0.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 0.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lTime.SetSecondDouble(1.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 90.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lTime.SetSecondDouble(2.0);
			lKeyIndex = lCurve->KeyAdd(lTime);
			lCurve->KeySetValue(lKeyIndex, 0.0);
			lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationCubic);
			lCurve->KeyModifyEnd();
		}
	}
}


DLLEXPORT bool ExportFile(char* filename)
{
	FbxExporter* exporter = FbxExporter::Create(manager, "");

	if (!exporter->Initialize(filename, -1, manager->GetIOSettings()))
	{
		std::cerr << "Failed to initialize the FBX exporter." << std::endl;
		exporter->Destroy();
		return false;
	}

	exporter->SetFileExportVersion(FBX_2014_00_COMPATIBLE);
	exporter->Export(scene);

	exporter->Destroy();
	return true;
}


DLLEXPORT void PrintMesh()
{
	std::cout << "meshes : " << custom_scene->meshCount << std::endl;

	for (int i = 0; i < custom_scene->meshCount; i++)
	{
		std::cout << "polygons : " << custom_scene->meshes[i].polygonCount << std::endl;

		for (int j = 0; j < custom_scene->meshes[i].polygonCount; j++)
		{
			std::cout << "polygonSize : " << custom_scene->meshes[i].polygons[j].polygonSize;
			std::cout << ", startIndex : " << custom_scene->meshes[i].polygons[j].index << std::endl;
		}
	}
}


DLLEXPORT void PrintBone()
{
    for (int i = 0; i < custom_scene->boneCount; i++)
    {
        std::cout << "Bone ID: " << custom_scene->bones[i].boneId;
        std::cout << ", Bone Name: " << custom_scene->bones[i].boneName << std::endl;
    }
}
