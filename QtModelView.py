from PyQt5 import QtWidgets, QtGui, QtOpenGL, QtCore
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from interface import *

class QtModelViewPort(QtOpenGL.QGLWidget):
    def __init__(self, parent=None):
        super(QtModelViewPort, self).__init__(parent)
        self.lastPos = QtGui.QVector2D()
        self.panX = 0.0
        self.panY = 0.0
        self.rotationX = -20.0
        self.rotationY = -20.0
        self.zoom = -100.0
        self.model = Model("F:/Major Project/3DAPE/3D models/Test.fbx")

    def initializeGL(self):
        glClearColor(0.2, 0.2, 0.2, 1.0)
        glEnable(GL_DEPTH_TEST)

    def resizeGL(self, width, height):
        glViewport(0, 0, width, height)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(45, width / float(height), 0.1, 100.0)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        glTranslatef(self.panX, self.panY, self.zoom)
        #gluLookAt(0, 0, self.zoom, 0.0, 0.0, 0, 0, 1, 0)
        glRotatef(self.rotationX, 1.0, 0.0, 0.0)
        glRotatef(self.rotationY, 0.0, 1.0, 0.0)

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)# gonna convert this to a variable to view different render models.
        
        for i in range(self.model.scene.contents.numofmeshes):
            vertices = self.model.meshes[i][0]
            indices = self.model.meshes[i][1]
            if self.model.scene.contents.meshes[i].typeofpolygon == 2:
                glBegin(GL_LINES)
                for vertexIndex in indices:
                    vertex = vertices[vertexIndex]
                    glVertex3f(vertex[0], vertex[1], vertex[2])
                glEnd()

            elif self.model.scene.contents.meshes[i].typeofpolygon == 3:
                glBegin(GL_TRIANGLES)
                for vertexIndex in indices:
                    vertex = vertices[vertexIndex]
                    glVertex3f(vertex[0], vertex[1], vertex[2])
                glEnd()

            elif self.model.scene.contents.meshes[i].typeofpolygon == 4:
                glBegin(GL_QUADS)
                for vertexIndex in indices:
                    vertex = vertices[vertexIndex]
                    glVertex3f(vertex[0], vertex[1], vertex[2])
                glEnd()

            elif self.model.scene.contents.meshes[i].typeofpolygon >= 5:
                glBegin(GL_POLYGON)
                for vertexIndex in indices:
                    vertex = vertices[vertexIndex]
                    glVertex3f(vertex[0], vertex[1], vertex[2])
                glEnd()

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
            self.panX += dx *0.01
            self.panY -= dy *0.01
            #print(self.panY)
            self.update()

        self.lastPos = QtGui.QVector2D(event.pos())

    def wheelEvent(self, event):
        delta = event.angleDelta().y() / 120
        self.zoom += delta * 0.5
        self.update()

    def changeFile(self, filename: str):
        glClearColor(0.2, 0.2, 0.2, 1.0)
        self.model = Model(filename)
        self.paintGL()

'''app = QtWidgets.QApplication([])
widget = QtModelViewPort()
widget.show()
app.exec()'''
