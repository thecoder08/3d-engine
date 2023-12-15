#include "engine-wl.h"
#define TINYOBJ_LOADER_C_IMPLEMENTATION 1
#include "tinyobj_loader_c.h"
#include <xgfx/window-wl.h>
#include <xgfx/drawing.h>
#define __USE_MISC 1
#include <math.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

static int width;
static int height;
static int *depthBuffer;
static vec3 lightPosition;
static int lightIntensity;

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
static void drawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, int depth) {
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
                if (x >= 0 && x < width && y >= 0 && y < height) {
                    if (depth < depthBuffer[y * width + x]) {
                        plot(x, y, color);
                        depthBuffer[y * width + x] = depth;
                    }
                }
            }
        }
    }
}

static void triCenter(vec3 a, vec3 b, vec3 c, vec3 center) {
    center[0] = (a[0] + b[0] + c[0]) / 3;
    center[1] = (a[1] + b[1] + c[1]) / 3;
    center[2] = (a[2] + b[2] + c[2]) / 3;
}

static float distance(vec3 object1, vec3 object2) {
    return sqrt((object2[0] - object1[0]) * (object2[0] - object1[0]) + (object2[1] - object1[1]) * (object2[1] - object1[1]) + (object2[2] - object1[2]) * (object2[2] - object1[2]));
}

static void (*draw3D)();

static void drawClear() {
    rectangle(0, 0, 640, 480, 0x00000000);
    for (int i=0; i<width*height; ++i) depthBuffer[i] = 0x7fffffff;
    draw3D();
}

void initEngine(int widthArg, int heightArg, const char* title, vec3 lightPositionArg, int lightIntensityArg, void (*draw_3d)(), void (*keychange)(unsigned int key, unsigned int state)) {
    draw3D = draw_3d;
    width = widthArg;
    height = heightArg;
    depthBuffer = malloc(width * height * sizeof(int));
    memcpy(lightPosition, lightPositionArg, sizeof(vec3));
    lightIntensity = lightIntensityArg;
    initWindow(width, height, title, drawClear, keychange);
}

int loadObj(const char* filename, vec3** vertices, int* numVertices, int** indices, int* numIndices) {
    // loads the specified object file
    tinyobj_attrib_t attrib;
    tinyobj_shape_t* shapes = NULL;
    size_t num_shapes;
    tinyobj_material_t* materials = NULL;
    size_t num_materials;
    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    if (tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials, &num_materials, filename, get_file_data, NULL, flags) != TINYOBJ_SUCCESS) {
        return 1;
    }
    *vertices = malloc(attrib.num_vertices * sizeof(vec3));
    *indices = malloc(attrib.num_faces * sizeof(tinyobj_vertex_index_t));
    for (int i = 0; i < attrib.num_vertices; i++) {
        (*vertices)[i][0] = attrib.vertices[i * 3];
        (*vertices)[i][1] = attrib.vertices[i * 3 + 1];
        (*vertices)[i][2] = attrib.vertices[i * 3 + 2];
    }
    for (int i = 0; i < attrib.num_faces; i++) {
        (*indices)[i] = attrib.faces[i].v_idx;
    }
    *numVertices = attrib.num_vertices;
    *numIndices = attrib.num_faces;
    return 0;
}

void renderObject(vec3* vertexBuffer, int vertexBufferLength, int* indexBuffer, int indexBufferLength, mat3 transformation, char wireframe) {
    vec3 transformedVertices[vertexBufferLength];
    // transform vertices
    for (int i = 0; i < vertexBufferLength; i++) {
        mul_vec3_mat3(vertexBuffer[i], transformation, transformedVertices[i]);
    }
    // draw vertices
    for (int i = 0; i < indexBufferLength; i += 3) {
        if (wireframe) {
            line((int)round(transformedVertices[indexBuffer[i]][0] * 120) + width / 2, (int)round(transformedVertices[indexBuffer[i]][1] * -120) + height / 2, (int)round(transformedVertices[indexBuffer[i + 1]][0] * 120) + width / 2, (int)round(transformedVertices[indexBuffer[i + 1]][1] * -120) + height / 2, 0x0000ffff);
            line((int)round(transformedVertices[indexBuffer[i]][0] * 120) + width / 2, (int)round(transformedVertices[indexBuffer[i]][1] * -120) + height / 2, (int)round(transformedVertices[indexBuffer[i + 2]][0] * 120) + width / 2, (int)round(transformedVertices[indexBuffer[i + 2]][1] * -120) + height / 2, 0x0000ffff);
            line((int)round(transformedVertices[indexBuffer[i + 2]][0] * 120) + width / 2, (int)round(transformedVertices[indexBuffer[i + 2]][1] * -120) + height / 2, (int)round(transformedVertices[indexBuffer[i + 1]][0] * 120) + width / 2, (int)round(transformedVertices[indexBuffer[i + 1]][1] * -120) + height / 2, 0x0000ffff);
        }
        else {
            int lightValueSum = 0;
            for (int j = 0; j < 3; j++) {
                unsigned char vertexLightValue = 0;
                if ((int)round(distance(lightPosition, transformedVertices[indexBuffer[i + j]]) * (100 / lightIntensity)) <= 255) {
                    vertexLightValue = 255 - (unsigned char)round(distance(lightPosition, transformedVertices[indexBuffer[i + j]]) * (100 / lightIntensity));
                }
                lightValueSum += vertexLightValue;
            }
            unsigned char lightValue = lightValueSum / 3;
            vec3 center;
            triCenter(transformedVertices[indexBuffer[i]], transformedVertices[indexBuffer[i + 1]], transformedVertices[indexBuffer[i + 2]], center);
            drawFilledTriangle((int)round(transformedVertices[indexBuffer[i]][0] * 120) + width / 2, (int)round(transformedVertices[indexBuffer[i]][1] * -120) + height / 2, (int)round(transformedVertices[indexBuffer[i + 1]][0] * 120) + width / 2, (int)round(transformedVertices[indexBuffer[i + 1]][1] * -120) + height / 2, (int)round(transformedVertices[indexBuffer[i + 2]][0] * 120) + width / 2, (int)round(transformedVertices[indexBuffer[i + 2]][1] * -120) + height / 2, (lightValue << 8) + lightValue, (int)(center[2] * 120));
        }
    }
}

int dispatch() {
    return dispatchEvents();
}