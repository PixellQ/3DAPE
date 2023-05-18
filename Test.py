import sys

from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5.Qt3DCore import QEntity, QTransform, QScene
from PyQt5.Qt3DRender import QCamera, QRenderSettings


class MainWindow(QWidget):

    def __init__(self):
        super().__init__()

        # Create a Qt3D scene.
        self.scene = QScene()

        # Add a 3D model to the scene.
        self.model = QEntity()
        self.model.addComponent(QTransform())
        self.scene.addEntity(self.model)

        # Set the camera and lighting.
        self.camera = QCamera()
        self.camera.setPosition(QVector3D(0, 0, 10))
        self.scene.addEntity(self.camera)

        self.light = QEntity()
        self.light.addComponent(QDirectionalLight())
        self.scene.addEntity(self.light)

        # Render the scene.
        self.renderSettings = QRenderSettings()
        self.renderSettings.setSamples(4)
        self.scene.setRenderSettings(self.renderSettings)

        # Create a viewport.
        self.viewport = QWidget()
        self.viewport.setScene(self.scene)

        # Layout the widgets.
        self.layout = QVBoxLayout()
        self.layout.addWidget(self.viewport)
        self.setLayout(self.layout)


if __name__ == "__main__":

    # Create an application.
    application = QApplication(sys.argv)

    # Create a main window.
    main_window = MainWindow()

    # Show the main window.
    main_window.show()

    # Start the application event loop.
    sys.exit(application.exec_())
