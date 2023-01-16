#include "rotate.h"
#include <math.h>

void mul_vec3_mat3(vec3 vector, mat3 matrix, vec3 dest) {
    dest[0] = (matrix[0][0] * vector[0]) + (matrix[0][1] * vector[1]) + (matrix[0][2] * vector[2]);
    dest[1] = (matrix[1][0] * vector[0]) + (matrix[1][1] * vector[1]) + (matrix[1][2] * vector[2]);
    dest[2] = (matrix[2][0] * vector[0]) + (matrix[2][1] * vector[1]) + (matrix[2][2] * vector[2]);
}

void rotateX(vec3 vertex, float angle, vec3 dest) {
    mat3 rotationMatrix;
    rotationMatrix[0][0] = 1; rotationMatrix[0][1] = 0; rotationMatrix[0][2] = 0;
    rotationMatrix[1][0] = 0; rotationMatrix[1][1] = cos(angle); rotationMatrix[1][2] = -sin(angle);
    rotationMatrix[2][0] = 0; rotationMatrix[2][1] = sin(angle); rotationMatrix[2][2] = cos(angle);
    mul_vec3_mat3(vertex, rotationMatrix, dest);
}

void rotateY(vec3 vertex, float angle, vec3 dest) {
    mat3 rotationMatrix;
    rotationMatrix[0][0] = cos(angle); rotationMatrix[0][1] = 0; rotationMatrix[0][2] = sin(angle);
    rotationMatrix[1][0] = 0; rotationMatrix[1][1] = 1; rotationMatrix[1][2] = 0;
    rotationMatrix[2][0] = -sin(angle); rotationMatrix[2][1] = 0; rotationMatrix[2][2] = cos(angle);
    mul_vec3_mat3(vertex, rotationMatrix, dest);
}

void rotateZ(vec3 vertex, float angle, vec3 dest) {
    mat3 rotationMatrix;
    rotationMatrix[0][0] = cos(angle); rotationMatrix[0][1] = -sin(angle); rotationMatrix[0][2] = 0;
    rotationMatrix[1][0] = sin(angle); rotationMatrix[1][1] = cos(angle); rotationMatrix[1][2] = 0;
    rotationMatrix[2][0] = 0; rotationMatrix[2][1] = 0; rotationMatrix[2][2] = 1;
    mul_vec3_mat3(vertex, rotationMatrix, dest);
}