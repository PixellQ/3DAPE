#pragma once

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