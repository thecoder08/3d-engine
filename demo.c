#include "rotate.h"
#include <xgfx/window.h>
#include <xgfx/drawing.h>
#define __USE_MISC 1
#include <math.h>
#include <unistd.h>

int main() {
    initWindow(640, 480, "3D engine");

    /*vec3 vertices[8];
    vertices[0][0] = 1; vertices[0][1] = 1; vertices[0][2] = 1;
    vertices[1][0] = -1; vertices[1][1] = 1; vertices[1][2] = 1;
    vertices[2][0] = 1; vertices[2][1] = -1; vertices[2][2] = 1;
    vertices[3][0] = -1; vertices[3][1] = -1; vertices[3][2] = 1;
    vertices[4][0] = 1; vertices[4][1] = 1; vertices[4][2] = -1;
    vertices[5][0] = -1; vertices[5][1] = 1; vertices[5][2] = -1;
    vertices[6][0] = 1; vertices[6][1] = -1; vertices[6][2] = -1;
    vertices[7][0] = -1; vertices[7][1] = -1; vertices[7][2] = -1;
    int indices[] = {1, 2, 0, 1, 2, 3, 5, 6, 4, 5, 6, 7, 1, 7, 3, 1, 7, 5, 4, 2, 0, 4, 2, 6, 0, 5, 1, 0, 5, 4, 2, 7, 3, 2, 7, 6};*/

    vec3 vertices[5];
    vertices[0][0] = 1; vertices[0][1] = 1; vertices[0][2] = 1;
    vertices[1][0] = -1; vertices[1][1] = 1; vertices[1][2] = 1;
    vertices[2][0] = 1; vertices[2][1] = 1; vertices[2][2] = -1;
    vertices[3][0] = -1; vertices[3][1] = 1; vertices[3][2] = -1;
    vertices[4][0] = 0; vertices[4][1] = -1; vertices[4][2] = 0;
    int indices[] = {0, 1, 2, 3, 1, 2, 4, 0, 1, 4, 1, 3, 4, 3, 2, 4, 2, 0};
    
    float angleX = 0;
    float angleY = 0;
    float angleZ = 0;
    int upPressed = 0;
    int downPressed = 0;
    int leftPressed = 0;
    int rightPressed = 0;
    int homePressed = 0;
    int endPressed = 0;
    while(1) {
        XEvent event;
        int result = checkWindowEvents(&event);
        if (result == 2) {
            break;
        }
        if (result == 1) {
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
        vec3 transformedVertices[sizeof(vertices)/sizeof(vertices[0])];
        for (int i = 0; i < sizeof(vertices)/sizeof(vertices[0]); i++) {
            vec3 tmp;
            vec3 tmp2;
            rotateX(vertices[i], angleX, tmp);
            rotateY(tmp, angleY, tmp2);
            rotateZ(tmp2, angleZ, transformedVertices[i]);
        }
        rectangle(0, 0, 640, 480, 0x00000000);
        for (int i = 0; i < sizeof(indices)/sizeof(indices[0]); i += 3) {
            line((int)round(transformedVertices[indices[i]][0] * 120) + 320, (int)round(transformedVertices[indices[i]][1] * 120) + 240, (int)round(transformedVertices[indices[i + 1]][0] * 120) + 320, (int)round(transformedVertices[indices[i + 1]][1] * 120) + 240, 0x0000ffff);
            line((int)round(transformedVertices[indices[i]][0] * 120) + 320, (int)round(transformedVertices[indices[i]][1] * 120) + 240, (int)round(transformedVertices[indices[i + 2]][0] * 120) + 320, (int)round(transformedVertices[indices[i + 2]][1] * 120) + 240, 0x0000ffff);
            line((int)round(transformedVertices[indices[i + 2]][0] * 120) + 320, (int)round(transformedVertices[indices[i + 2]][1] * 120) + 240, (int)round(transformedVertices[indices[i + 1]][0] * 120) + 320, (int)round(transformedVertices[indices[i + 1]][1] * 120) + 240, 0x0000ffff);
        }
        updateWindow();
        usleep(16667);
    }
    return 0;
}