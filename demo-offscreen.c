#include "engine-offscreen.h"
#define __USE_MISC 1
#include <math.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>

int framebuffer[640 * 480];

int main(int argc, char** argv) {
    vec3* vertices;
    int numVertices;
    int* indices;
    int numIndices;
    float angleX = 0;
    float angleY = 0;
    float angleZ = 0;
    int upPressed = 0;
    int downPressed = 1;
    int leftPressed = 0;
    int rightPressed = 1;
    int homePressed = 0;
    int endPressed = 0;
    int wireframe = 0;
    vec3 lightPosition = {2, 2, -2};
    int lightIntensity = 2;
    if (argc < 2) {
        printf("You must provide an object argument!\n");
        return 1;
    }
    if (loadObj(argv[1], &vertices, &numVertices, &indices, &numIndices)) {
        printf("Failed to load object file!\n");
        return 1;
    }
    initEngine(framebuffer, 640, 480, "3D Engine", lightPosition, lightIntensity);
    int file = open("offscreen.data", O_WRONLY|O_CREAT, 0664);
    for (int i = 0; i < 120; i++) {
        if (upPressed) {
            angleX += 0.0625;
        }
        if (downPressed) {
            angleX -= 0.0625;
        }
        if (leftPressed) {
            angleY += 0.0625;
        }
        if (rightPressed) {
            angleY -= 0.0625;
        }
        if (homePressed) {
            angleZ += 0.0625;
        }
        if (endPressed) {
            angleZ -= 0.0625;
        }
        if (angleX >= M_PI * 2) {
                angleX -= M_PI * 2;
        }
        if (angleY >= M_PI * 2) {
                angleX -= M_PI * 2;
        }
        if (angleZ >= M_PI * 2) {
                angleX -= M_PI * 2;
        }
        clear3D();
        mat3 rotationMatrixA;
        rotationMatrixXYZ(angleX, angleY, angleZ, rotationMatrixA);
        renderObject(vertices, numVertices, indices, numIndices, rotationMatrixA, wireframe);
        write(file, framebuffer, sizeof(framebuffer));
    }
    close(file);
    return 0;
}
