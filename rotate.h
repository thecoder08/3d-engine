typedef float vec3[3];
typedef vec3 mat3[3];

void mul_vec3_mat3(vec3 vector, mat3 matrix, vec3 dest);
void rotationMatrixXYZ(float angleX, float angleY, float angleZ, mat3 rotationMatrix);