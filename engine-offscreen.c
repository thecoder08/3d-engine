#include "engine-offscreen.h"
#define TINYOBJ_LOADER_C_IMPLEMENTATION 1
#include "tinyobj_loader_c.h"
#define __USE_MISC 1
#include <math.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int width;
static int height;
static int *depthBuffer;
static int *fb;
static vec3 lightPosition;
static int lightIntensity;

void plot(int x, int y, int color) {
    color = 0xff000000 + ((color & 0x00ff0000)>>16) + (color & 0x0000ff00) + ((color & 0x000000ff)<<16);
  if (x >= 0 && x < width && y >= 0 && y < height) {
    fb[y*width + x] = color;
  }
}

void rectangle(int x, int y, int width, int height, int color) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      plot(x + j, y + i, color);
    }
  }
}

void line(int x0, int y0, int x1, int y1, int color) {
   int dx = abs(x1 - x0);
   int dy = abs(y1 - y0);
   int sx = (x0 < x1) ? 1 : -1;
   int sy = (y0 < y1) ? 1 : -1;
   int err = dx - dy;
   while(1) {
      plot(x0, y0, color);
      if ((x0 == x1) && (y0 == y1)) break;
      int e2 = 2*err;
      if (e2 > -dy) { err -= dy; x0  += sx; }
      if (e2 < dx) { err += dx; y0  += sy; }
   }
}

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

static float distancef(vec3 object1, vec3 object2) {
    return sqrtf((object2[0] - object1[0]) * (object2[0] - object1[0]) + (object2[1] - object1[1]) * (object2[1] - object1[1]) + (object2[2] - object1[2]) * (object2[2] - object1[2]));
}

float max(float x, float y) { return (x>y) ? x : y; }
float min(float x, float y) { return (x<y) ? x : y; }

float edgeFunction(vec3 a, vec3 b, vec3 c) {
    return ((c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]));
}

float dot(vec3 a, vec3 b) {
    return a[0]*b[0] + a[1]*b[1];
}

void getColor(vec3 a, vec3 v1color, vec3 b, vec3 v2color, vec3 c, vec3 v3color, vec3 p, vec3 color) {
    vec3 v0;
    v0[0] = b[0] - a[0];
    v0[1] = b[1] - a[1];
    //v0[2] = b[2] - a[2];
    vec3 v1;
    v1[0] = c[0] - a[0];
    v1[1] = c[1] - a[1];
    //v1[2] = c[2] - a[2];
    vec3 v2;
    v2[0] = p[0] - a[0];
    v2[1] = p[1] - a[1];
    //v2[2] = p[2] - a[2];

    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    color[0] = v1color[0]*u + v2color[0]*v + v3color[0]*w;
    color[1] = v1color[1]*u + v2color[1]*v + v3color[1]*w;
    color[2] = v1color[2]*u + v2color[2]*v + v3color[2]*w;
}

void drawTriangle(vec3 v1, vec3 color1, vec3 v2, vec3 color2, vec3 v3, vec3 color3, int depth) {
    for (int i = min(min(v1[1]*(-120)+(height/2), v2[1]*(-120)+(height/2)), v3[1]*(-120)+(height/2)); i < max(max(v1[1]*(-120)+(height/2), v2[1]*(-120)+(height/2)), v3[1]*(-120)+(height/2)); i++) {
        for (int j = min(min(v1[0]*120+(width/2), v2[0]*120+(width/2)), v3[0]*120+(width/2)); j < max(max(v1[0]*120+(width/2), v2[0]*120+(width/2)), v3[0]*120+(width/2)); j++) {
            //plot(j, i, 0x00808080);
            if (i < 0 || i >= height || j < 0 || j >= width) {
                continue;
            }
            if (depth >= depthBuffer[i * width + j]) {
                continue;
            }
            vec3 sample;
            sample[0] = (j-(width/2))/120.f;
            sample[1] = (i-(height/2))/-120.f;
            if (((edgeFunction(v1, v2, sample) >= 0) && (edgeFunction(v2, v3, sample) >= 0) && (edgeFunction(v3, v1, sample) >= 0)) || ((edgeFunction(v1, v2, sample) <= 0) && (edgeFunction(v2, v3, sample) <= 0) && (edgeFunction(v3, v1, sample) <= 0))) {
                vec3 pixelColor;
                getColor(v1, color1, v2, color2, v3, color3, sample, pixelColor);
                depthBuffer[i * width + j] = depth;
                plot(j, i, (unsigned int)(pixelColor[2]*255) + ((unsigned int)(pixelColor[1]*255)<<8) + ((unsigned int)(pixelColor[0]*255)<<16));
            }
        }
    }
}

static void triCenter(vec3 a, vec3 b, vec3 c, vec3 center) {
    center[0] = (a[0] + b[0] + c[0]) / 3;
    center[1] = (a[1] + b[1] + c[1]) / 3;
    center[2] = (a[2] + b[2] + c[2]) / 3;
}

void initEngine(int* framebuffer, int widthArg, int heightArg, const char* title, vec3 lightPositionArg, int lightIntensityArg) {
    fb = framebuffer;
    width = widthArg;
    height = heightArg;
    depthBuffer = malloc(width * height * sizeof(int));
    memcpy(lightPosition, lightPositionArg, sizeof(vec3)); //{2, 2, -2};
    lightIntensity = lightIntensityArg; //2;
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
            float vertexLightValue1 = 0;
            if ((int)round(distancef(lightPosition, transformedVertices[indexBuffer[i]]) * (100 / lightIntensity)) <= 255) {
                vertexLightValue1 = 255 - (unsigned char)round(distancef(lightPosition, transformedVertices[indexBuffer[i]]) * (100 / lightIntensity));
            }
            vec3 color1;
            color1[0] = 0;
            color1[1] = vertexLightValue1/255;
            color1[2] = vertexLightValue1/255;
            float vertexLightValue2 = 0;
            if ((int)round(distancef(lightPosition, transformedVertices[indexBuffer[i + 1]]) * (100 / lightIntensity)) <= 255) {
                vertexLightValue2 = 255 - (unsigned char)round(distancef(lightPosition, transformedVertices[indexBuffer[i + 1]]) * (100 / lightIntensity));
            }
            vec3 color2;
            color2[0] = 0;
            color2[1] = vertexLightValue2/255;
            color2[2] = vertexLightValue2/255;
            float vertexLightValue3 = 0;
            if ((int)round(distancef(lightPosition, transformedVertices[indexBuffer[i + 2]]) * (100 / lightIntensity)) <= 255) {
                vertexLightValue3 = 255 - (unsigned char)round(distancef(lightPosition, transformedVertices[indexBuffer[i + 2]]) * (100 / lightIntensity));
            }
            vec3 color3;
            color3[0] = 0;
            color3[1] = vertexLightValue3/255;
            color3[2] = vertexLightValue3/255;

            if (indexBuffer[i] == 0) {
                color1[0] = 1;
                color1[1] = 0;
                color1[2] = 0;
            }
            if (indexBuffer[i + 1] == 0) {
                color2[0] = 1;
                color2[1] = 0;
                color2[2] = 0;
            }
            if (indexBuffer[i + 2] == 0) {
                color3[0] = 1;
                color3[1] = 0;
                color3[2] = 0;
            }

            vec3 center;
            triCenter(transformedVertices[indexBuffer[i]], transformedVertices[indexBuffer[i + 1]], transformedVertices[indexBuffer[i + 2]], center);
            drawTriangle(transformedVertices[indexBuffer[i]], color1, transformedVertices[indexBuffer[i + 1]], color2, transformedVertices[indexBuffer[i + 2]], color3, (int)(center[2] * 120));
        }
    }
}

void clear3D() {
    rectangle(0, 0, 640, 480, 0x00000000);
    for (int i=0; i<width*height; ++i) depthBuffer[i] = 0x7fffffff;
}