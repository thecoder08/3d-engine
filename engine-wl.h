#include "rotate.h"
void initEngine(int widthArg, int heightArg, const char* title, vec3 lightPositionArg, int lightIntensityArg, void (*draw_3d)(), void (*keychange)(unsigned int key, unsigned int state));
int loadObj(const char* filename, vec3** vertices, int* numVertices, int** indices, int* numIndices);
void renderObject(vec3* vertexBuffer, int vertexBufferLength, int* indexBuffer, int indexBufferLength, mat3 transformation, char wireframe);
int dispatch();