#include "engine.h"
#define __USE_MISC 1
#include <math.h>
#include <xgfx/window.h>
#include <stdio.h>

int main(int argc, char** argv) {
    vec3** vertices;
    vec3** vertexNormals;
    int numMaterials;
    int* numVertices;
    vec3* colors;
    float angleX = 0;
    float angleY = 0;
    float angleZ = 0;
    int upPressed = 0;
    int downPressed = 0;
    int leftPressed = 0;
    int rightPressed = 0;
    int homePressed = 0;
    int endPressed = 0;
    int wireframe = 0;
    vec3 lightPosition = {2, 2, 2};
    int lightIntensity = 2;
    if (argc < 2) {
        printf("You must provide an object argument!\n");
        return 1;
    }
    if (loadObj(argv[1], &numMaterials, &colors, &vertices, &vertexNormals, &numVertices)) {
        printf("Failed to load object file!\n");
        return 1;
    }
    initEngine(640, 480, "3D Engine", lightPosition, lightIntensity);
    while(1) {
        Event event;
        while (checkWindowEvent(&event)) {
            if (event.type == WINDOW_CLOSE) {
                return 0;
            }
            if (event.type == KEY_CHANGE) {
                if (event.keychange.key == 103) {
                    upPressed = event.keychange.state;
                }
                if (event.keychange.key == 108) {
                    downPressed = event.keychange.state;
                }
                if (event.keychange.key == 105) {
                    leftPressed = event.keychange.state;
                }
                if (event.keychange.key == 106) {
                    rightPressed = event.keychange.state;
                }
                if (event.keychange.key == 102) {
                    homePressed = event.keychange.state;
                }
                if (event.keychange.key == 107) {
                    endPressed = event.keychange.state;
                }
                if (event.keychange.key == 1 && event.keychange.state) {
                    if (wireframe) {
                        wireframe = 0;
                    }
                    else {
                        wireframe = 1;
                    }
                }
            }
        }

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
                angleY -= M_PI * 2;
        }
        if (angleZ >= M_PI * 2) {
                angleZ -= M_PI * 2;
        }
        if (angleX <= M_PI * 2) {
                angleX += M_PI * 2;
        }
        if (angleY <= M_PI * 2) {
                angleY += M_PI * 2;
        }
        if (angleZ <= M_PI * 2) {
                angleZ += M_PI * 2;
        }
        mat3 rotationMatrixA;
        rotationMatrixXYZ(angleX, angleY, angleZ, rotationMatrixA);
        for (int i = 0; i < numMaterials; i++) {
            renderObject(vertices[i], vertexNormals[i], numVertices[i], rotationMatrixA, colors[i], wireframe);
        }
        updateWindow3D();
    }
}
