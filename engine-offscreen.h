#include "rotate.h"
void initEngine(int* framebuffer, int widthArg, int heightArg, const char* title, vec3 lightPositionArg, int lightIntensityArg);
int loadObj(const char* filename, vec3** vertices, int* numVertices, int** indices, int* numIndices);
void renderObject(vec3* vertexBuffer, int vertexBufferLength, int* indexBuffer, int indexBufferLength, mat3 transformation, char wireframe);
void clear3D();
