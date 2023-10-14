#include "rotate.h"
#include <math.h>

void mul_vec3_mat3(vec3 vector, mat3 matrix, vec3 dest) {
    dest[0] = (matrix[0][0] * vector[0]) + (matrix[0][1] * vector[1]) + (matrix[0][2] * vector[2]);
    dest[1] = (matrix[1][0] * vector[0]) + (matrix[1][1] * vector[1]) + (matrix[1][2] * vector[2]);
    dest[2] = (matrix[2][0] * vector[0]) + (matrix[2][1] * vector[1]) + (matrix[2][2] * vector[2]);
}

void rotationMatrixXYZ(float angleX, float angleY, float angleZ, mat3 rotationMatrix) {
    rotationMatrix[0][0] = cos(angleY)*cos(angleZ); rotationMatrix[0][1] = sin(angleX)*sin(angleY)*cos(angleZ)-cos(angleX)*sin(angleZ); rotationMatrix[0][2] = cos(angleX)*sin(angleY)*cos(angleZ)+sin(angleX)*sin(angleZ);
    rotationMatrix[1][0] = cos(angleY)*sin(angleZ); rotationMatrix[1][1] = sin(angleX)*sin(angleY)*sin(angleZ)+cos(angleX)*cos(angleZ); rotationMatrix[1][2] = cos(angleX)*sin(angleY)*sin(angleZ)-sin(angleX)*cos(angleZ);
    rotationMatrix[2][0] = -sin(angleY); rotationMatrix[2][1] = sin(angleX)*cos(angleY); rotationMatrix[2][2] = cos(angleX)*cos(angleY);
}