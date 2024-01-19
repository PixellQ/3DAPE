from ctypes import *
import os

class Vertex(Structure):
    _fields_ = [('x', c_float), ('y', c_float), ('z', c_float)]

class Polygon(Structure):
    _fields_ = [('polygonSize', c_int), ('index', c_int)]

class Mesh(Structure):
    _fields_ = [('polygonCount', c_int), ('polygons', POINTER(Polygon)), ('vertexCount', c_int), 
                ('vertices', POINTER(Vertex)), ('indexCount', c_int), ('indices', POINTER(c_int)), 
                ('normalCount', c_int), ('normals', POINTER(Vertex))]

class Bone(Structure):
    _fields_ = [('boneId', c_int), ('boneName', POINTER(c_char_p))]

class Scene(Structure):
    _fields_ = [('meshCount', c_int), ('meshes', POINTER(Mesh)), ('boneCount', c_int), ('bones', POINTER(Bone))]


class Model():
    def __init__(self, filename):

        if filename:

            rootPath = os.getcwd()
            self.fbxLayer = CDLL(os.path.join(rootPath, "M0dl.dll"))

            encoded_file = filename.encode('utf-8')
            openingCase = self.fbxLayer.OpenFile(encoded_file)

            if openingCase == 0 : pass
            elif openingCase == 1 : print("No Skeleton Present !")

            self.fbxLayer.GetSceneDetails.restype = POINTER(Scene)
            self.scene = self.fbxLayer.GetSceneDetails()

            #fbxLayer.PrintBone()

            self.meshes = []
            

            for i in range(self.scene.contents.boneCount):
                #print(self.scene.contents.bones[i].boneId)
                print(string_at(self.scene.contents.bones[i].boneName).decode("utf-8"))

            self.fbxLayer.AnimateRotation(3)

            for i in range(self.scene.contents.meshCount): 
                polygons = []
                vertices = []
                indices = []
                normals = []
                for j in range(self.scene.contents.meshes[i].polygonCount):
                    polygons.append((self.scene.contents.meshes[i].polygons[j].polygonSize, self.scene.contents.meshes[i].polygons[j].index))
                for j in range(self.scene.contents.meshes[i].vertexCount):
                    vertices.append((self.scene.contents.meshes[i].vertices[j].x, self.scene.contents.meshes[i].vertices[j].y, self.scene.contents.meshes[i].vertices[j].z))
                for j in range(self.scene.contents.meshes[i].indexCount):
                    indices.append(self.scene.contents.meshes[i].indices[j])
                for j in range(self.scene.contents.meshes[i].normalCount):
                    normals.append((self.scene.contents.meshes[i].normals[j].x, self.scene.contents.meshes[i].normals[j].y, self.scene.contents.meshes[i].normals[j].z))
                self.meshes.append([polygons, vertices, indices, normals])


    def exportFile(self, fileloc):
        if fileloc:
            encoded_location = fileloc.encode('utf-8')
            self.fbxLayer.ExportFile(encoded_location)