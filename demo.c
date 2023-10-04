#include "rotate.h"
#define TINYOBJ_LOADER_C_IMPLEMENTATION 1
#include "tinyobj_loader_c.h"
#include <xgfx/window.h>
#include <xgfx/drawing.h>
#define __USE_MISC 1
#include <math.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define EVENT_BUFFER_SIZE 100
XEvent eventBuffer[EVENT_BUFFER_SIZE];

#define WIDTH 640
#define HEIGHT 480
int depthBuffer[WIDTH * HEIGHT];

// the below is from the tinyobjectloader-c on github
static void get_file_data(void* ctx, const char* filename, const int is_mtl,
                          const char* obj_filename, char** data, size_t* len) {
  // NOTE: If you allocate the buffer with malloc(),
  // You can define your own memory management struct and pass it through `ctx`
  // to store the pointer and free memories at clean up stage(when you quit an
  // app)
  // This example uses mmap(), so no free() required.
  (void)ctx;

  if (!filename) {
    (*data) = NULL;
    (*len) = 0;
    return;
  }
  struct stat sb;

  int fd = open(filename, O_RDONLY);

  fstat(fd, &sb);

  (*data) = (char*)mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

  (*len) = sb.st_size;
}

// from ChatGPT
void drawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, int depth)
{
    int minX = fmin(fmin(x1, x2), x3);
    int maxX = fmax(fmax(x1, x2), x3);
    int minY = fmin(fmin(y1, y2), y3);
    int maxY = fmax(fmax(y1, y2), y3);

    int area = ((x2 - x1) * (y3 - y1)) - ((y2 - y1) * (x3 - x1));

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            int w1 = ((x2 - x1) * (y - y1)) - ((y2 - y1) * (x - x1));
            int w2 = ((x3 - x2) * (y - y2)) - ((y3 - y2) * (x - x2));
            int w3 = ((x1 - x3) * (y - y3)) - ((y1 - y3) * (x - x3));

            if ((w1 >= 0 && w2 >= 0 && w3 >= 0) || (w1 <= 0 && w2 <= 0 && w3 <= 0))
            {
                if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                    if (depth < depthBuffer[y * WIDTH + x]) {
                        plot(x, y, color);
                        depthBuffer[y * WIDTH + x] = depth;
                    }
                }
            }
        }
    }
}

void triCenter(vec3 a, vec3 b, vec3 c, vec3 center) {
    center[0] = (a[0] + b[0] + c[0]) / 3;
    center[1] = (a[1] + b[1] + c[1]) / 3;
    center[2] = (a[2] + b[2] + c[2]) / 3;
}

float distance(vec3 object1, vec3 object2) {
    return sqrt((object2[0] - object1[0]) * (object2[0] - object1[0]) + (object2[1] - object1[1]) * (object2[1] - object1[1]) + (object2[2] - object1[2]) * (object2[2] - object1[2]));
}

int main(int argc, char** argv) {
    initWindow(WIDTH, HEIGHT, "3D engine");

    // loads the specified object file
    tinyobj_attrib_t attrib;
    tinyobj_shape_t* shapes = NULL;
    size_t num_shapes;
    tinyobj_material_t* materials = NULL;
    size_t num_materials;
    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    if (tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials, &num_materials, argv[1], get_file_data, NULL, flags) != TINYOBJ_SUCCESS) {
        printf("Failed to load object file!\n");
        return 0;
    }
    vec3 vertices[attrib.num_vertices];
    for (int i = 0; i < attrib.num_vertices; i++) {
        vertices[i][0] = attrib.vertices[i * 3];
        vertices[i][1] = attrib.vertices[i * 3 + 1];
        vertices[i][2] = attrib.vertices[i * 3 + 2];
    }

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
        vec3 transformedVertices[attrib.num_vertices];
        for (int i = 0; i < attrib.num_vertices; i++) {
            vec3 tmp;
            vec3 tmp2;
            rotateX(vertices[i], angleX, tmp);
            rotateY(tmp, angleY, tmp2);
            rotateZ(tmp2, angleZ, transformedVertices[i]);
        }
        rectangle(0, 0, 640, 480, 0x00000000);
        for (int i=0; i<sizeof(depthBuffer)/sizeof(depthBuffer[0]); ++i) depthBuffer[i] = 0x7fffffff;
        for (int i = 0; i < attrib.num_faces; i += 3) {
            if (wireframe) {
                line((int)round(transformedVertices[attrib.faces[i].v_idx][0] * 120) + WIDTH / 2, (int)round(transformedVertices[attrib.faces[i].v_idx][1] * -120) + HEIGHT / 2, (int)round(transformedVertices[attrib.faces[i + 1].v_idx][0] * 120) + WIDTH / 2, (int)round(transformedVertices[attrib.faces[i + 1].v_idx][1] * -120) + HEIGHT / 2, 0x0000ffff);
                line((int)round(transformedVertices[attrib.faces[i].v_idx][0] * 120) + WIDTH / 2, (int)round(transformedVertices[attrib.faces[i].v_idx][1] * -120) + HEIGHT / 2, (int)round(transformedVertices[attrib.faces[i + 2].v_idx][0] * 120) + WIDTH / 2, (int)round(transformedVertices[attrib.faces[i + 2].v_idx][1] * -120) + HEIGHT / 2, 0x0000ffff);
                line((int)round(transformedVertices[attrib.faces[i + 2].v_idx][0] * 120) + WIDTH / 2, (int)round(transformedVertices[attrib.faces[i + 2].v_idx][1] * -120) + HEIGHT / 2, (int)round(transformedVertices[attrib.faces[i + 1].v_idx][0] * 120) + WIDTH / 2, (int)round(transformedVertices[attrib.faces[i + 1].v_idx][1] * -120) + HEIGHT / 2, 0x0000ffff);
            }
            else {
                int lightValueSum = 0;
                for (int j = 0; j < 3; j++) {
                    unsigned char vertexLightValue = 0;
                    if ((int)round(distance(lightPosition, transformedVertices[attrib.faces[i + j].v_idx]) * (100 / lightIntensity)) <= 255) {
                        vertexLightValue = 255 - (unsigned char)round(distance(lightPosition, transformedVertices[attrib.faces[i + j].v_idx]) * (100 / lightIntensity));
                    }
                    lightValueSum += vertexLightValue;
                }
                unsigned char lightValue = lightValueSum / 3;
                vec3 center;
                triCenter(transformedVertices[attrib.faces[i].v_idx], transformedVertices[attrib.faces[i + 1].v_idx], transformedVertices[attrib.faces[i + 2].v_idx], center);
                drawFilledTriangle((int)round(transformedVertices[attrib.faces[i].v_idx][0] * 120) + WIDTH / 2, (int)round(transformedVertices[attrib.faces[i].v_idx][1] * -120) + HEIGHT / 2, (int)round(transformedVertices[attrib.faces[i + 1].v_idx][0] * 120) + WIDTH / 2, (int)round(transformedVertices[attrib.faces[i + 1].v_idx][1] * -120) + HEIGHT / 2, (int)round(transformedVertices[attrib.faces[i + 2].v_idx][0] * 120) + WIDTH / 2, (int)round(transformedVertices[attrib.faces[i + 2].v_idx][1] * -120) + HEIGHT / 2, (lightValue << 8) + lightValue, (int)(center[2] * 120));
            }
        }
        updateWindow();
    }
}
