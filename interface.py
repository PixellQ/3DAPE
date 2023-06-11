from ctypes import *
import os

class Vertex(Structure):
    _fields_ = [('x', c_float), ('y', c_float), ('z', c_float)]

class Mesh(Structure):
    _fields_ = [('numofvertices', c_int), ('typeofpolygon', c_int), ('vertices', POINTER(Vertex)), ('indices', POINTER(c_int)), ('numofindices', c_int), ('numofnormals', c_int), ('normals', POINTER(Vertex))]

class Bone(Structure):
    _fields_ = [('id', c_int), ('name', POINTER(c_char_p))]

class Scene(Structure):
    _fields_ = [('numofmeshes', c_int), ('numofbones', c_int), ('meshes', POINTER(Mesh)), ('bones', POINTER(Bone))]

class Model():
    def __init__(self, filename):
        pwd = os.getcwd()
        lib = CDLL(os.path.join(pwd, "M0dl.dll"))
        lib.openfile.restype = POINTER(Scene)
        encoded_file = filename.encode('utf-8')
        self.scene = lib.openfile(encoded_file)
        #lib.printscene()

        self.meshes = []

        for i in range(self.scene.contents.numofmeshes): 
            vertices = []
            indices = []
            normals = []
            for j in range(self.scene.contents.meshes[i].numofvertices):
                vertices.append((self.scene.contents.meshes[i].vertices[j].x, self.scene.contents.meshes[i].vertices[j].y, self.scene.contents.meshes[i].vertices[j].z))
            for j in range(self.scene.contents.meshes[i].numofindices):
                indices.append(self.scene.contents.meshes[i].indices[j])
            for j in range(self.scene.contents.meshes[i].numofnormals):
                normals.append((self.scene.contents.meshes[i].normals[j].x, self.scene.contents.meshes[i].normals[j].y, self.scene.contents.meshes[i].normals[j].z))
            self.meshes.append([vertices, indices, normals])
            #print(self.scene.contents.meshes[i].numofnormals)
            #print(self.scene.contents.meshes[i].numofvertices)
            #print(self.scene.contents.meshes[i].numofindices)
        '''
        bones = []
        for i in range(scene.contents.numofbones):
            bones.append((scene.contents.bones[i].id, scene.contents.bones[i].name))

        lib.updatescene.argtypes = [c_double]'''
        #lib.exportscene(b"sample.obj")


