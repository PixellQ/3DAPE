from PyQt5 import QtWidgets, QtGui, QtOpenGL, QtCore
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from ctest import Model

class QtModelViewPort(QtOpenGL.QGLWidget):
    def __init__(self, parent=None):
        super(QtModelViewPort, self).__init__(parent)

        self.setAutoFillBackground(True)
        palette = self.palette()
        palette.setColor(self.backgroundRole(), QtGui.QColor(255, 255, 255, 255))
        self.setPalette(palette)

        self.lastPos = QtGui.QVector2D()
        self.panX = 0.0
        self.panY = 0.0
        self.rotationX = -20.0
        self.rotationY = -20.0
        self.zoom = -100.0
        self.model = Model("")
        self.setAttribute(QtCore.Qt.WA_AlwaysStackOnTop)

    def initializeGL(self):
        #glShadeModel(GL_SMOOTH)
        glClearColor(0.2, 0.2, 0.2, 1.0)
        glEnable(GL_DEPTH_TEST)

    def resizeGL(self, width, height):
        glViewport(0, 0, width, height)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(45, width / float(height), 0.1, 250.0)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        glTranslatef(self.panX, self.panY, self.zoom)
        #gluLookAt(0, 0, self.zoom, 0.0, 0.0, 0, 0, 1, 0)
        glRotatef(self.rotationX, 1.0, 0.0, 0.0)
        glRotatef(self.rotationY, 0.0, 1.0, 0.0)

        glEnable(GL_LIGHTING)  # Enable lighting
        glEnable(GL_LIGHT0)  # Enable light source 0

        # Set light source position
        light_pos = [1.0, 1.0, 1.0, 0.0]  # (x, y, z, w)
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos)

        # Set light source properties
        light_ambient = [0.2, 0.2, 0.2, 1.0]
        light_diffuse = [1.0, 1.0, 1.0, 1.0]
        light_specular = [1.0, 1.0, 1.0, 1.0]
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse)
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular)

        # Set material properties of the cube
        material_ambient = [0.1, 0.1, 0.1, 1.0]
        material_diffuse = [0.8, 0.8, 0.8, 1.0]
        material_specular = [1.0, 1.0, 1.0, 1.0]
        material_shininess = [50.0]
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient)
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse)
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular)
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess)

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)# gonna convert this to a variable to view different render models.
        
        for meNo in range(self.model.scene.contents.meshCount):
            polygons = self.model.meshes[meNo][0]
            vertices = self.model.meshes[meNo][1]
            indices = self.model.meshes[meNo][2]
            normals = self.model.meshes[meNo][3]

            #print(self.model.scene.contents.meshes[meNo].indexCount)
            #print(self.model.scene.contents.meshes[meNo].normalCount)

            for poNo in range(self.model.scene.contents.meshes[meNo].polygonCount):

                polygonSize = polygons[poNo][0]
                startIndex = polygons[poNo][1]

                if polygonSize == 2: drawing_mode = GL_LINES
                elif polygonSize == 3: drawing_mode = GL_TRIANGLES
                elif polygonSize == 4: drawing_mode = GL_QUADS
                elif polygonSize >= 5: drawing_mode = GL_POLYGON

                glBegin(drawing_mode)
                for vertIndex in range(startIndex, startIndex + polygonSize):
                    index = indices[vertIndex]

                    vertex = vertices[index]
                    glVertex3f(vertex[0], vertex[1], vertex[2])

                    normal = normals[vertIndex]
                    glNormal3f(normal[0], normal[1], normal[2])
                glEnd()

            glDisable(GL_LIGHTING)

        glFlush()

    def mousePressEvent(self, event):
        self.lastPos = QtGui.QVector2D(event.pos())

    def mouseMoveEvent(self, event):
        dx = event.x() - self.lastPos.x()
        dy = event.y() - self.lastPos.y()

        if event.buttons() & QtCore.Qt.LeftButton:
            self.rotationX -= dy * 0.2
            self.rotationY += dx * 0.2
            self.update()

        elif event.buttons() & QtCore.Qt.RightButton:
            self.panX += dx *0.05
            self.panY -= dy *0.05
            self.update()

        self.lastPos = QtGui.QVector2D(event.pos())

    def wheelEvent(self, event):
        delta = event.angleDelta().y() / 120
        self.zoom += delta * 0.5
        self.update()

    def changeFile(self, filename: str):
        #glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self.model = Model(filename)
        #self.initializeGL()
        self.update()

'''app = QtWidgets.QApplication([])
widget = QtModelViewPort()
widget.show()
app.exec()'''
