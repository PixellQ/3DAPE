import sys
from PyQt5.QtCore import Qt, QUrl
from PyQt5.QtGui import QSurfaceFormat
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QFileDialog
from PyQt5.Qt3DCore import QEntity
from PyQt5.Qt3DExtras import Qt3DWindow, QPhongMaterial, QMesh, QOrbitCameraController


class MainWindow(QWidget):
    def __init__(self):
        super().__init__()

        # Set up the Qt3D window
        format = QSurfaceFormat()
        format.setSamples(4)
        QSurfaceFormat.setDefaultFormat(format)
        self.view = Qt3DWindow()
        self.container = QWidget.createWindowContainer(self.view)
        self.container.setMinimumSize(800, 600)
        self.view.setContainer(self.container)
        self.view.defaultFrameGraph().setClearColor(Qt.white)

        # Set up the file dialog
        self.file_dialog = QFileDialog()
        self.file_dialog.setNameFilter("FBX files (*.fbx)")

        # Create the entity
        self.entity = QEntity()
        self.view.sceneRoot().addChild(self.entity)

        # Create the material
        material = QPhongMaterial(self.entity)
        material.setDiffuse(Qt.white)
        material.setShininess(100.0)

        # Set up the camera controller
        camera_entity = self.view.camera()
        camera_controller = QOrbitCameraController(self.entity)
        camera_controller.setCamera(camera_entity)
        camera_controller.setLookSpeed(100.0)
        camera_controller.setLinearSpeed(50.0)

        # Set up the layout
        layout = QVBoxLayout()
        layout.addWidget(self.container)
        self.setLayout(layout)
        self.setWindowTitle("PyQt5 Qt3D Example")
        self.show()

        # Load the initial model
        self.load_model()

    def load_model(self):
        # Open the file dialog to select the model file
        file_name, _ = self.file_dialog.getOpenFileName(self, "Select Model File", "", "FBX files (*.fbx)")
        if file_name:
            # Remove the existing mesh
            for component in self.entity.components:
                if isinstance(component, QMesh):
                    self.entity.removeComponent(component)
            # Create the mesh
            mesh = QMesh()
            mesh.setSource(QUrl.fromLocalFile(file_name))
            self.entity.addComponent(mesh)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    sys.exit(app.exec_())
pp = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())