#include "engine-wl.h"
#define __USE_MISC 1
#include <math.h>
#include <stdio.h>

int upPressed = 0;
int downPressed = 0;
int leftPressed = 0;
int rightPressed = 0;
int homePressed = 0;
int endPressed = 0;
int wireframe = 0;

vec3* vertices;
int numVertices;
int* indices;
int numIndices;
mat3 rotationMatrixA;

void draw() {
    renderObject(vertices, numVertices, indices, numIndices, rotationMatrixA, wireframe);
}

void key_change(unsigned int key, unsigned int state) {
    if (key == 103) {
        upPressed = state;
    }
    if (key == 108) {
        downPressed = state;
    }
    if (key == 105) {
        leftPressed = state;
    }
    if (key == 106) {
        rightPressed = state;
    }
    if (key == 102) {
        homePressed = state;
    }
    if (key == 107) {
        endPressed = state;
    }
    if (state == 1 && key == 1) {
        if (wireframe) {
            wireframe = 0;
        }
        else {
            wireframe = state;
        }
    }
}

int main(int argc, char** argv) {
    float angleX = 0;
    float angleY = 0;
    float angleZ = 0;
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
    initEngine(640, 480, "3D Engine", lightPosition, lightIntensity, draw, key_change);
    while(dispatch() != -1) {
        if (upPressed) {
            angleX += 0.04;
        }
        if (downPressed) {
            angleX -= 0.04;
        }
        if (leftPressed) {
            angleY += 0.04;
        }
        if (rightPressed) {
            angleY -= 0.04;
        }
        if (homePressed) {
            angleZ += 0.04;
        }
        if (endPressed) {
            angleZ -= 0.04;
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
        rotationMatrixXYZ(angleX, angleY, angleZ, rotationMatrixA);
    }
}