// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

import QtQuick 2.15
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.15

Rectangle {
    id: rec
    visible: true
    anchors.fill: parent

    Scene3D {
        anchors.fill: parent
        id: modelscene
        focus: true
        hoverEnabled: true
        aspects: ["input", "logic", "render"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

        Entity {
            id: sceneRoot

            Camera {
                id: camera
                projectionType: CameraLens.PerspectiveProjection
                fieldOfView: 30
                aspectRatio: 16/9
                nearPlane: 0.1
                farPlane: 1000.0
                position: Qt.vector3d(2.0, 2.0, 6.0)
                upVector: Qt.vector3d(0.0, 1.0, 0.0)
                viewCenter: Qt.vector3d(0.0, 1.0, 0.0)
            }

            OrbitCameraController {
                camera: camera
            }

            components: [
                RenderSettings {
                    activeFrameGraph: ForwardRenderer {
                        clearColor: "#29292b"
                        camera: camera
                    }
                },
                InputSettings {}
            ]

            Entity {
                id: monkeyEntity
                property string meshSource: "Test.obj"
                components: [
                    Mesh {
                        id: mesh
                        source: monkeyEntity.meshSource
                    },
                    PhongMaterial {
                        id: material
                        ambient: "gray"
                        diffuse: "white"
                        specular: "white"
                        shininess: 50.0
                    }
                ]
            }

            Entity {
                id: planeEntity
                components: [
                    PlaneMesh {
                        id: planeMesh
                        width: 10.0
                        height: 10.0
                    },
                    Transform {
                        // position the plane slightly below the monkey model
                        translation: Qt.vector3d(0.0, 0.0, 0.0)
                    },
                    PhongMaterial {
                        // specify surface properties of the plane
                        ambient: "gray"
                        diffuse: "white"
                        specular: "white"
                        shininess: 50.0
                    }
                ]
            }
        }
    }
}
