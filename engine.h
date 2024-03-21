#include "rotate.h"
void initEngine(int widthArg, int heightArg, const char* title, vec3 lightPositionArg, int lightIntensityArg);
int loadObj(const char* filename, int* numMaterials, vec3** colors, vec3*** vertexPositions, vec3*** vertexNormals, int** numVertices);
void renderObject(vec3* vertexBuffer, vec3* vertexNormals, int vertexBufferLength, mat3 transformation, vec3 color, char wireframe);
void updateWindow3D();