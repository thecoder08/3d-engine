#include "engine.h"
#define __USE_MISC 1
#include <math.h>
#include <xgfx/window.h>
#include <stdio.h>

#define EVENT_BUFFER_SIZE 100
XEvent eventBuffer[EVENT_BUFFER_SIZE];

int main(int argc, char** argv) {
    vec3* vertices;
    int numVertices;
    int* indices;
    int numIndices;
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
    initEngine(640, 480, "3D Engine", lightPosition, lightIntensity);
    while(1) {
        int eventsRead = checkWindowEvents(eventBuffer, EVENT_BUFFER_SIZE);
        for (int i = 0; i < eventsRead; i++) {
            XEvent event = eventBuffer[i];
            if (event.type == ClosedWindow) {
                return 0;
            }
            if (event.type == KeyPress) {
                if (event.xkey.keycode == 111) {
                    upPressed = 1;
                }
                if (event.xkey.keycode == 116) {
                    downPressed = 1;
                }
                if (event.xkey.keycode == 113) {
                    leftPressed = 1;
                }
                if (event.xkey.keycode == 114) {
                    rightPressed = 1;
                }
                if (event.xkey.keycode == 110) {
                    homePressed = 1;
                }
                if (event.xkey.keycode == 115) {
                    endPressed = 1;
                }
                if (event.xkey.keycode == 9) {
                    if (wireframe) {
                        wireframe = 0;
                    }
                    else {
                        wireframe = 1;
                    }
                }
            }
            if (event.type == KeyRelease) {
                if (event.xkey.keycode == 111) {
                    upPressed = 0;
                }
                if (event.xkey.keycode == 116) {
                    downPressed = 0;
                }
                if (event.xkey.keycode == 113) {
                    leftPressed = 0;
                }
                if (event.xkey.keycode == 114) {
                    rightPressed = 0;
                }
                if (event.xkey.keycode == 110) {
                    homePressed = 0;
                }
                if (event.xkey.keycode == 115) {
                    endPressed = 0;
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
                angleX -= M_PI * 2;
        }
        if (angleZ >= M_PI * 2) {
                angleX -= M_PI * 2;
        }
        mat3 rotationMatrixA;
        rotationMatrixXYZ(angleX, angleY, angleZ, rotationMatrixA);
        renderObject(vertices, numVertices, indices, numIndices, rotationMatrixA, wireframe);
        updateWindow3D();
    }
}
