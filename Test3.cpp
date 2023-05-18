#include <fbxsdk.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace FBXSDK;

class Model {
public:
    Model(const std::string& filename) {
        // Load the model from file.
        FbxManager* manager = FbxManager::Create();
        FbxImporter* importer = FbxImporter::Create(manager, "");
        importer->Initialize(filename.c_str(), -1, manager->GetIOSettings());

        // Create the scene.
        FbxScene* scene = FbxScene::Create(manager, "");
        importer->Import(scene);

        // Get the number of meshes.
        int num_meshes = scene->GetMeshCount();

        // Create the vertices and faces.
        vertices.resize(num_meshes * scene->GetMesh(0)->GetPolygonCount() * 3);
        faces.resize(num_meshes * scene->GetMesh(0)->GetPolygonCount());

        // Iterate over the meshes.
        for (int i = 0; i < num_meshes; i++) {
            // Get the mesh.
            FbxMesh* mesh = scene->GetMesh(i);

            // Iterate over the vertices.
            for (int j = 0; j < mesh->GetPolygonCount(); j++) {
                // Get the vertex.
                FbxVector4 vertex = mesh->GetPolygonVertex(j, 0);

                // Add the vertex to the vertices array.
                vertices[i * mesh->GetPolygonCount() * 3 + j * 3] = vertex.x;
                vertices[i * mesh->GetPolygonCount() * 3 + j * 3 + 1] = vertex.y;
                vertices[i * mesh->GetPolygonCount() * 3 + j * 3 + 2] = vertex.z;
            }

            // Iterate over the faces.
            for (int j = 0; j < mesh->GetPolygonCount(); j++) {
                // Get the face.
                FbxPolygon* polygon = mesh->GetPolygon(j);

                // Add the face to the faces array.
                faces[i * mesh->GetPolygonCount() + j] = polygon->GetIndex(0);
                faces[i * mesh->GetPolygonCount() + j + 1] = polygon->GetIndex(1);
                faces[i * mesh->GetPolygonCount() + j + 2] = polygon->GetIndex(2);
            }
        }

        // Get the number of animations.
        int num_animations = scene->GetAnimationCount();

        // Create the animations.
        animations.resize(num_animations);
        for (int i = 0; i < num_animations; i++) {
            animations[i] = new FbxAnimStack(scene->GetAnimation(i));
        }
    }

    // Get the vertices.
    std::vector<float>& get_vertices() {
        return vertices;
    }

    // Get the faces.
    std::vector<int>& get_faces() {
        return faces;
    }

    // Get the animations.
    std::vector<FbxAnimStack*>& get_animations() {
        return animations;
    }

private:
    // The vertices.
    std::vector<float> vertices;

    // The faces.
    std::vector<int> faces;

    // The animations.
    std::vector<FbxAnimStack*> animations;
};

void display() {
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind the model.
    glBindVertexArray(model->get_vao());

    // Draw the model.
    glDrawElements(GL_TRIANGLES, model->get_num_faces() * 3, GL_UNSIGNED_INT, 0);

    // Unbind the model.
    glBindVertexArray(0);

    // Swap the buffers.
    glutSwapBuffers();
}

void reshape(int width, int height) {
    // Set the viewport.
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    // Initialize GLUT.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("3D Model with Animation");

    // Initialize OpenGL.
    glewInit();

    // Create the model.
    Model model("my_model.fbx");

    // Set the display function.
    glutDisplayFunc(display);

    // Set the reshape function.
    glutReshapeFunc(reshape);

    // Start GLUT.
    glutMainLoop();

    // Clean up.
    FbxManager::Destroy();

    return 0;
}
