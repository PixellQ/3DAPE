import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *
from interface import *

def Cube(i):
    vertices = meshes[i][0]
    normals = meshes[i][2]  # Vertex normals
    indices = meshes[i][1]
    type = GL_POINTS
    if scene.contents.meshes[i].typeofpolygon == 1:
        type = GL_POINTS
    elif scene.contents.meshes[i].typeofpolygon == 2:
        type = GL_LINES
    elif scene.contents.meshes[i].typeofpolygon == 3:
        type = GL_TRIANGLES
    elif scene.contents.meshes[i].typeofpolygon >= 4:
        type = GL_POLYGON

    vertices_flat = [v for vertex in vertices for v in vertex]
    normals_flat = [n for normal in normals for n in normal]

    # Create VBO for vertices
    vbo = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, len(vertices_flat) * 9, (GLfloat * len(vertices_flat))(*vertices_flat), GL_STATIC_DRAW)

    # Create VBO for normals
    #vbo_normals = glGenBuffers(1)
    #glBindBuffer(GL_ARRAY_BUFFER, vbo_normals)
    #glBufferData(GL_ARRAY_BUFFER, len(normals_flat) * 9, (GLfloat * len(normals_flat))(*normals_flat), GL_STATIC_DRAW)




    ebo = glGenBuffers(1)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, len(indices) * 4, (GLint * len(indices))(*indices), GL_STATIC_DRAW)

    # Enable vertex and normal arrays and specify their format
    glEnableClientState(GL_VERTEX_ARRAY)
    glEnableClientState(GL_NORMAL_ARRAY)
    glVertexPointer(3, GL_FLOAT, 0, None)
    #glNormalPointer(GL_FLOAT, 0, None)

    # Draw the mesh
    glDrawElements(type, len(indices), GL_UNSIGNED_INT, None)

    # Disable vertex and normal arrays and clean up
    glDisableClientState(GL_VERTEX_ARRAY)
   # glDisableClientState(GL_NORMAL_ARRAY)
    glBindBuffer(GL_ARRAY_BUFFER, 0)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)
    #glDeleteBuffers(1, [vbo, vbo_normals])
    glDeleteBuffers(1, [ebo])

time = 0.0

def main():
    lib.printscene()
    pygame.init()
    display = (800, 600)
    pygame.display.set_mode(display, DOUBLEBUF | OPENGL)

    gluPerspective(90, (display[0] / display[1]), 0.1, 50.0)

    glTranslatef(0.0, 0.0, -5.0)

    rotation_speed = 0.2
    pan_speed = 0.01

    is_panning = False
    start_pan_pos = None

    clock = pygame.time.Clock()

    while True:
        dt = clock.tick(60) / 1000.0

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 4:  # Scroll up to zoom in
                    glTranslatef(0.0, 0.0, 1.0)
                elif event.button == 5:  # Scroll down to zoom out
                    glTranslatef(0.0, 0.0, -1.0)

            if event.type == pygame.MOUSEMOTION:
                if event.buttons[0] == 1:  # Left mouse button to rotate
                    x, y = event.rel
                    glRotatef(x * rotation_speed, 0, 1, 0)
                    glRotatef(y * rotation_speed, 1, 0, 0)
                elif event.buttons[2] == 1:  # Right mouse button to pan
                    if is_panning:
                        x, y = event.pos
                        dx = x - start_pan_pos[0]
                        dy = y - start_pan_pos[1]
                        glTranslatef(-dx * pan_speed, dy * pan_speed, 0.0)

            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 3:  # Middle mouse button to start panning
                is_panning = True
                start_pan_pos = event.pos

            if event.type == pygame.MOUSEBUTTONUP and event.button == 3:  # Middle mouse button to stop panning
                is_panning = False

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        # Render your 3D scene here
        for i in range(len(meshes)):
            Cube(i)

        pygame.display.flip()

main()

