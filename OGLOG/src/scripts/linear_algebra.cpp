#include "linear_algebra.h";
#include <IOStream>

mat4 create_matrix_transformation(vec3 translation)
{
    mat4 matrix;

    for (int i = 0; i < 12; i++)    { matrix.entries[i] = 0.0f; }

    for (int i = 0; i < 3; i++)     { matrix.entries[i + 12] = translation.entries[i]; }

    for (int i = 0; i < 16; i += 5) { matrix.entries[i] = 1; }

    return matrix;
}