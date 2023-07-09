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

Scene* test_scene;

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

	test_scene = sceneDetails;
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


DLLEXPORT void PrintCount()
{
	std::cout << "meshes : " << test_scene->meshCount << std::endl;

    for (int i = 0; i < test_scene->meshCount; i++)
    {
        std::cout << "polygons : " << test_scene->meshes[i].polygonCount << std::endl;

        for (int j = 0; j < test_scene->meshes[i].polygonCount; j++)
        {
            std::cout << "polygonSize : " << test_scene->meshes[i].polygons[j].polygonSize;
            std::cout << ", startIndex : " << test_scene->meshes[i].polygons[j].index << std::endl;
        }
    }

    for (int i = 0; i < test_scene->boneCount; i++)
    {
        std::cout << "Bone ID: " << test_scene->bones[i].boneId;
        std::cout << ", Bone Name: " << test_scene->bones[i].boneName << std::endl;
    }
}
