#include <cmath>

struct mat4 {
    float entries[16];
};

struct vec3 {
    float entries[3];
};

mat4 create_matrix_transformation(vec3 translation);

mat4 create_z_rotation(float angle);