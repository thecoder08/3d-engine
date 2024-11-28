#include "engine.h"
#define TINYOBJ_LOADER_C_IMPLEMENTATION 1
#include "tinyobj_loader_c.h"
#include <xgfx/window.h>
#include <xgfx/drawing.h>
#define __USE_MISC 1
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

static int width;
static int height;
static int* depthBuffer;
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

  (*data) = malloc(sb.st_size);
  read(fd, *data, sb.st_size);


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
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

void normalize(vec3 vector) {
    float length = sqrtf(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
    vector[0] /= length;
    vector[1] /= length;
    vector[2] /= length;
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

void initEngine(int widthArg, int heightArg, const char* title, vec3 lightPositionArg, int lightIntensityArg) {
    width = widthArg;
    height = heightArg;
    initWindow(width, height, title);
    depthBuffer = malloc(width * height * sizeof(int));
    memcpy(lightPosition, lightPositionArg, sizeof(vec3)); //{2, 2, -2};
    lightIntensity = lightIntensityArg; //2;
}

int loadObj(const char* filename, int* numMaterials, vec3** colors, vec3*** vertexPositions, vec3*** vertexNormals, int** numVertices) {
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
    if (num_materials == 0) { // default material; cyan color
        num_materials = 1;
        materials = malloc(sizeof(tinyobj_material_t));
        materials[0].diffuse[0] = 0;
        materials[0].diffuse[1] = 1;
        materials[0].diffuse[2] = 1;
        attrib.material_ids = calloc(attrib.num_faces, sizeof(int));
    }
    *numMaterials = num_materials;
    printf("%s has %ld materials\n", filename, num_materials);
    printf("%s has %d faces\n", filename, attrib.num_face_num_verts);
    printf("%s has %d vertices\n", filename, attrib.num_vertices);
    
    *colors = malloc(num_materials * sizeof(vec3));
    *numVertices = malloc(num_materials * sizeof(int));
    *vertexPositions = malloc(num_materials * sizeof(vec3**));
    *vertexNormals = malloc(num_materials * sizeof(vec3**));
    for (int i = 0; i < num_materials; i++) {
        (*colors)[i][0] = materials[i].diffuse[0];
        (*colors)[i][1] = materials[i].diffuse[1];
        (*colors)[i][2] = materials[i].diffuse[2];

        int verticesUsing = 0;
        for (int j = 0; j < attrib.num_face_num_verts; j++) {
            if (attrib.material_ids[j] == i) {
                verticesUsing += 3; // this may need to be += 3
            }
        }
        (*numVertices)[i] = verticesUsing;

        int foundIndex = 0;
        (*vertexPositions)[i] = malloc(verticesUsing * sizeof(vec3));
        (*vertexNormals)[i] = malloc(verticesUsing * sizeof(vec3));
        for (int j = 0; j < attrib.num_face_num_verts; j++) {
            if (attrib.material_ids[j] == i) {
                for (int k = 0; k < 3; k++) {
                    (*vertexPositions)[i][foundIndex][0] = attrib.vertices[attrib.faces[j * 3 + k].v_idx * 3];
                    (*vertexPositions)[i][foundIndex][1] = attrib.vertices[attrib.faces[j * 3 + k].v_idx * 3 + 1];
                    (*vertexPositions)[i][foundIndex][2] = attrib.vertices[attrib.faces[j * 3 + k].v_idx * 3 + 2];
                    if (attrib.num_normals > 0) {
                        (*vertexNormals)[i][foundIndex][0] = attrib.normals[attrib.faces[j * 3 + k].vn_idx * 3];
                        (*vertexNormals)[i][foundIndex][1] = attrib.normals[attrib.faces[j * 3 + k].vn_idx * 3 + 1];
                        (*vertexNormals)[i][foundIndex][2] = attrib.normals[attrib.faces[j * 3 + k].vn_idx * 3 + 2];
                    }
                    else { // default normals: all the same direction
                        (*vertexNormals)[i][foundIndex][0] = 1;
                        (*vertexNormals)[i][foundIndex][1] = 0;
                        (*vertexNormals)[i][foundIndex][2] = 0;
                    }
                    foundIndex++;
                }
            }
        }
    }

    return 0;
}

void renderObject(vec3* vertexBuffer, vec3* vertexNormals, int vertexBufferLength, mat3 transformation, vec3 color, char wireframe) {
    vec3 colors[vertexBufferLength];
    vec3 transformedVertices[vertexBufferLength];
    // transform vertices
    for (int i = 0; i < vertexBufferLength; i++) {
        vec3 transformedNormal;
        mul_vec3_mat3(vertexNormals[i], transformation, transformedNormal);

        normalize(lightPosition);
        float intensity = max(dot(transformedNormal, lightPosition), 0.3);
        colors[i][0] = color[0] * intensity;
        colors[i][1] = color[1] * intensity;
        colors[i][2] = color[2] * intensity;
        mul_vec3_mat3(vertexBuffer[i], transformation, transformedVertices[i]);
        //transformedVertices[i][0] *= 10;
        //transformedVertices[i][1] *= 10;
        //transformedVertices[i][2] *= 10;
    }
    // draw vertices
    for (int i = 0; i < vertexBufferLength; i += 3) {
        if (wireframe) {
            line((int)round(transformedVertices[i][0] * 120) + width / 2, (int)round(transformedVertices[i][1] * -120) + height / 2, (int)round(transformedVertices[i + 1][0] * 120) + width / 2, (int)round(transformedVertices[i + 1][1] * -120) + height / 2, (unsigned int)(color[2]*255) + ((unsigned int)(color[1]*255)<<8) + ((unsigned int)(color[0]*255)<<16));
            line((int)round(transformedVertices[i][0] * 120) + width / 2, (int)round(transformedVertices[i][1] * -120) + height / 2, (int)round(transformedVertices[i + 2][0] * 120) + width / 2, (int)round(transformedVertices[i + 2][1] * -120) + height / 2, (unsigned int)(color[2]*255) + ((unsigned int)(color[1]*255)<<8) + ((unsigned int)(color[0]*255)<<16));
            line((int)round(transformedVertices[i + 2][0] * 120) + width / 2, (int)round(transformedVertices[i + 2][1] * -120) + height / 2, (int)round(transformedVertices[i + 1][0] * 120) + width / 2, (int)round(transformedVertices[i + 1][1] * -120) + height / 2, (unsigned int)(color[2]*255) + ((unsigned int)(color[1]*255)<<8) + ((unsigned int)(color[0]*255)<<16));
        }
        else {
            float vertexLightValue1 = 0;
            if ((int)round(distancef(lightPosition, transformedVertices[i]) * (100 / lightIntensity)) <= 255) {
                vertexLightValue1 = 255 - (unsigned char)round(distancef(lightPosition, transformedVertices[i]) * (100 / lightIntensity));
            }
            vec3 color1;
            color1[0] = 0;
            color1[1] = vertexLightValue1/255;
            color1[2] = vertexLightValue1/255;
            float vertexLightValue2 = 0;
            if ((int)round(distancef(lightPosition, transformedVertices[i + 1]) * (100 / lightIntensity)) <= 255) {
                vertexLightValue2 = 255 - (unsigned char)round(distancef(lightPosition, transformedVertices[i + 1]) * (100 / lightIntensity));
            }
            vec3 color2;
            color2[0] = 0;
            color2[1] = vertexLightValue2/255;
            color2[2] = vertexLightValue2/255;
            float vertexLightValue3 = 0;
            if ((int)round(distancef(lightPosition, transformedVertices[i + 2]) * (100 / lightIntensity)) <= 255) {
                vertexLightValue3 = 255 - (unsigned char)round(distancef(lightPosition, transformedVertices[i + 2]) * (100 / lightIntensity));
            }
            vec3 color3;
            color3[0] = 0;
            color3[1] = vertexLightValue3/255;
            color3[2] = vertexLightValue3/255;

            color1[0] = 1;
            color1[1] = 1;
            color1[2] = 0;
            color2[0] = 0;
            color2[1] = 1;
            color2[2] = 1;
            color3[0] = 1;
            color3[1] = 0;
            color3[2] = 1;

            vec3 center;
            triCenter(transformedVertices[i], transformedVertices[i + 1], transformedVertices[i + 2], center);
            drawTriangle(transformedVertices[i], colors[i], transformedVertices[i + 1], colors[i + 1], transformedVertices[i + 2], colors[i + 2], (int)(center[2] * 120));
        }
    }
}

void updateWindow3D() {
    updateWindow();
    rectangle(0, 0, width, height, 0x00000000);
    for (int i=0; i<width*height; ++i) depthBuffer[i] = 0x7fffffff;
}
