#include <iostream>
#include <vector>
#include <fbxsdk.h>
#include <stdlib.h>

#define DLLEXPORT extern "C" __declspec(dllexport)

std::vector<FbxMesh*> meshes;
std::vector<FbxSkeleton*> bones;

FbxManager* manager;
FbxScene* scene;

struct Vertex {
	float x;
	float y;
	float z;
};

struct Mesh {
	int numofvertices;
	int typeofpolygon;
	Vertex* vertices;
	int* indices;
	int numofindices;
	int numofnormals;
	Vertex* normals;
};

struct Bone {
	int id;
	const char* name;
};

struct Scene {
	int numofmeshes;
	int numofbones;
	Mesh* meshes;
	Bone* bones;
};

Scene* cscene;

DLLEXPORT void collectmeshes(FbxNode* parent) {
	FbxMesh* mesh = parent->GetMesh();
	if (mesh != NULL)
		meshes.push_back(mesh);
	else{
		FbxSkeleton* skeleton = parent->GetSkeleton();
		if (skeleton != NULL)
			bones.push_back(skeleton);
	}
	for (int i = 0; i < parent->GetChildCount(); i++) {
		collectmeshes(parent->GetChild(i));
	}
}


DLLEXPORT Scene* getscene() {
	Mesh* mesh = (Mesh*)malloc(sizeof(Mesh) * meshes.size());
	Bone* bone = (Bone*)malloc(sizeof(Bone) * bones.size());
	Scene* scenetoreturn = (Scene*)malloc(sizeof(Scene));
	//Vertex* normals;
	for (int i = 0; i < meshes.size(); i++) {
		int numofcontrolpoints = meshes[i]->GetControlPointsCount();
		//FbxTime time;
		//time.SetSecondDouble(10);
		//FbxAMatrix globaltransform = meshes[i]->GetNode()->EvaluateGlobalTransform();
		int count = meshes[i]->GetPolygonVertexCount();
		Vertex* vertices = (Vertex*)malloc(sizeof(Vertex) * numofcontrolpoints);
		int* indices = (int*)malloc(sizeof(int) * count);
		FbxVector4* controlpoints = meshes[i]->GetControlPoints();
		for (int j = 0; j < numofcontrolpoints; j++) {
			//FbxVector4 transformation = globaltransform.MultT(controlpoints[j]);
			FbxVector4 transformation = controlpoints[j];
			//transformation = globaltransform.MultR(transformation);
			//transformation = globaltransform.MultS(transformation);
			vertices[j].x = transformation.mData[0];
			vertices[j].y = transformation.mData[1];
			vertices[j].z = transformation.mData[2];
		}
		int typeofpoly = meshes[i]->GetPolygonSize(0);
		int* index = meshes[i]->GetPolygonVertices();
			
		mesh[i].numofvertices = numofcontrolpoints;
		mesh[i].typeofpolygon = typeofpoly;
		mesh[i].vertices = vertices;
		mesh[i].numofindices = count;
		FbxArray<FbxVector4> normalsarray;
		meshes[i]->GetPolygonVertexNormals(normalsarray);
		mesh[i].numofnormals = normalsarray.Size();
		Vertex* normals = (Vertex*)malloc(sizeof(Vertex) * normalsarray.Size());
		for (int j = 0; j < normalsarray.Size(); j++) {
			FbxVector4 transformation = normalsarray[j];
			normals[j].x = transformation.mData[0];
			normals[j].y = transformation.mData[1];
			normals[j].z = transformation.mData[2];
		}
		mesh[i].normals = normals;
		for(size_t j = 0; j < count; j++) indices[j] = index[j];
		mesh[i].indices = indices ;

	}

	for (int i = 0; i < bones.size(); i++) {
		bone[i].id = i;
		bone[i].name = bones[i]->GetName();
		//memcpy(bone[i].name, bones[i]->GetName(), sizeof(bones[i]->GetName())/sizeof(char));
	}
	scenetoreturn->meshes = mesh;
	scenetoreturn->numofmeshes = meshes.size();
	scenetoreturn->numofbones = bones.size();
	scenetoreturn->bones = bone;
	return scenetoreturn;
}

DLLEXPORT Scene *openfile(char* filename) {
	manager = FbxManager::Create();
	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename, -1, manager->GetIOSettings());
	scene = FbxScene::Create(manager, "");
	importer->Import(scene);
	importer->Destroy();
	collectmeshes(scene->GetRootNode());
	cscene = getscene();

	/*if (scene)
	{
		scene->Clear();
	}*/

	return cscene;
}

DLLEXPORT void printscene() {
	Mesh* scenemeshes = cscene->meshes;
	for (int i = 0; i < cscene->numofmeshes; i++) {
		std::cout << "Mesh " << i + 1 << std::endl;
		std::cout << "Num of vertices : " << scenemeshes[i].numofvertices << std::endl;
		std::cout << "Num of normals : " << scenemeshes[i].numofnormals << std::endl;
		std::cout << "Size of polygons : " << scenemeshes[i].typeofpolygon << std::endl;
		for (int j = 0; j < scenemeshes[i].numofindices; j++) {
			std::cout << "Index : " << j + 1 << " : " << scenemeshes[i].indices[j] << std::endl;
		}
		std::cout << std::endl;
		for (int j = 0; j < scenemeshes[i].numofvertices; j++) {
			std::cout << "X : " << scenemeshes[i].vertices[j].x;
			std::cout << "\tY : " << scenemeshes[i].vertices[j].y;
			std::cout << "\tZ : " << scenemeshes[i].vertices[j].z;
			std::cout << "\n";
		}
		std::cout << std::endl;
		for (int j = 0; j < scenemeshes[i].numofnormals; j++) {
			std::cout << "X : " << scenemeshes[i].normals[j].x;
			std::cout << "\tY : " << scenemeshes[i].normals[j].y;
			std::cout << "\tZ : " << scenemeshes[i].normals[j].z;
			std::cout << "\n";
		}
		std::cout << std::endl;
	}

	for (int i = 0; i < cscene->numofbones; i++) {
		std::cout << "Bone : " << cscene->bones[i].id + 1 << " : " << cscene->bones[i].name<<std::endl;
	}
	std::cout << std::endl;
}

DLLEXPORT void exportscene(char* filename) {
	FbxExporter* exporter = FbxExporter::Create(manager, "");
	exporter->Initialize(filename, -1, manager->GetIOSettings());
	exporter->Export(scene);
	exporter->Destroy();
}

	
void transformControlPoints(FbxMesh* mesh, FbxTime time, int meshindex)
{
	int numControlPoints = mesh->GetControlPointsCount();
	FbxVector4* controlPoints = mesh->GetControlPoints();
	

	for (int i = 0; i < mesh->GetDeformerCount(); i++)
	{
		FbxDeformer* deformer = mesh->GetDeformer(i);
		if (deformer->GetDeformerType() == FbxDeformer::eSkin)
		{
			FbxSkin* skin = (FbxSkin*)deformer;
			for (int j = 0; j < skin->GetClusterCount(); j++)
			{
				FbxCluster* cluster = skin->GetCluster(j);
				FbxNode* bone = cluster->GetLink();

				FbxAMatrix boneTransform = bone->EvaluateGlobalTransform(time);
				FbxAMatrix vertexTransform;
				cluster->GetTransformLinkMatrix(vertexTransform);
				FbxAMatrix transform = vertexTransform * boneTransform;

				for (int k = 0; k < cluster->GetControlPointIndicesCount(); k++)
				{
					int index = cluster->GetControlPointIndices()[k];
					double weight = cluster->GetControlPointWeights()[k];
					FbxVector4 transformedControlPoint = transform.MultT(controlPoints[index]);
					controlPoints[index] = controlPoints[index] * (1.0 - weight) + transformedControlPoint * weight;
				}
			}
		}
	}
	for (int i = 0; i < numControlPoints; i++) {
		cscene->meshes[meshindex].vertices[i].x = controlPoints[i].mData[0];
		cscene->meshes[meshindex].vertices[i].y = controlPoints[i].mData[1];
		cscene->meshes[meshindex].vertices[i].z = controlPoints[i].mData[2];
	}

}


DLLEXPORT void updatescene(double time_) {
	FbxTime time;
	time.SetSecondDouble(time_);
	for(int i=0; i<meshes.size(); i++) {
		transformControlPoints(meshes[i], time, i);
	}
}
