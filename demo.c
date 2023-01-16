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
    float angle = 0;
    while(1) {
        vec3 newCube[8];
        for (int i = 0; i < 8; i++) {
            vec3 tmp;
            rotateX(cube[i], angle, tmp);
            rotateY(tmp, angle, newCube[i]);
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
        
        angle += 0.0625;
        if (angle >= M_PI * 2) {
            angle -= M_PI * 2;
        }
        usleep(10000);
    }
    return 0;
}