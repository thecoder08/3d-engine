#include "rotate.h"
#include <xgfx/window.h>
#include <xgfx/drawing.h>
#include <math.h>
#include <unistd.h>

int main() {
    initWindow(640, 480, "Cube");
    vec3 cube[8];
    cube[0][0] = 1; cube[0][1] = 1; cube[0][2] = 1;
    cube[1][0] = -1; cube[1][1] = 1; cube[1][2] = 1;
    cube[2][0] = 1; cube[2][1] = -1; cube[2][2] = 1;
    cube[3][0] = -1; cube[3][1] = -1; cube[3][2] = 1;
    cube[4][0] = 1; cube[4][1] = 1; cube[4][2] = -1;
    cube[5][0] = -1; cube[5][1] = 1; cube[5][2] = -1;
    cube[6][0] = 1; cube[6][1] = -1; cube[6][2] = -1;
    cube[7][0] = -1; cube[7][1] = -1; cube[7][2] = -1;
    float angleX = 0;
    float angleY = 0;
    float angleZ = 0;
    int upPressed;
    int downPressed;
    int leftPressed;
    int rightPressed;
    int homePressed;
    int endPressed;
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
        vec3 newCube[8];
        for (int i = 0; i < 8; i++) {
            vec3 tmp;
            vec3 tmp2;
            rotateX(cube[i], angleX, tmp);
            rotateY(tmp, angleY, tmp2);
            rotateZ(tmp2, angleZ, newCube[i]);
        }

        rectangle(0, 0, 640, 480, 0x00000000);
        line((int)round(newCube[0][0] * 120) + 320, (int)round(newCube[0][1] * 120) + 240, (int)round(newCube[1][0] * 120) + 320, (int)round(newCube[1][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[1][0] * 120) + 320, (int)round(newCube[1][1] * 120) + 240, (int)round(newCube[3][0] * 120) + 320, (int)round(newCube[3][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[3][0] * 120) + 320, (int)round(newCube[3][1] * 120) + 240, (int)round(newCube[2][0] * 120) + 320, (int)round(newCube[2][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[2][0] * 120) + 320, (int)round(newCube[2][1] * 120) + 240, (int)round(newCube[0][0] * 120) + 320, (int)round(newCube[0][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[4][0] * 120) + 320, (int)round(newCube[4][1] * 120) + 240, (int)round(newCube[5][0] * 120) + 320, (int)round(newCube[5][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[5][0] * 120) + 320, (int)round(newCube[5][1] * 120) + 240, (int)round(newCube[7][0] * 120) + 320, (int)round(newCube[7][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[7][0] * 120) + 320, (int)round(newCube[7][1] * 120) + 240, (int)round(newCube[6][0] * 120) + 320, (int)round(newCube[6][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[6][0] * 120) + 320, (int)round(newCube[6][1] * 120) + 240, (int)round(newCube[4][0] * 120) + 320, (int)round(newCube[4][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[0][0] * 120) + 320, (int)round(newCube[0][1] * 120) + 240, (int)round(newCube[4][0] * 120) + 320, (int)round(newCube[4][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[1][0] * 120) + 320, (int)round(newCube[1][1] * 120) + 240, (int)round(newCube[5][0] * 120) + 320, (int)round(newCube[5][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[3][0] * 120) + 320, (int)round(newCube[3][1] * 120) + 240, (int)round(newCube[7][0] * 120) + 320, (int)round(newCube[7][1] * 120) + 240, 0x0000ffff);
        line((int)round(newCube[2][0] * 120) + 320, (int)round(newCube[2][1] * 120) + 240, (int)round(newCube[6][0] * 120) + 320, (int)round(newCube[6][1] * 120) + 240, 0x0000ffff);
        updateWindow();
        usleep(10000);
    }
    return 0;
}