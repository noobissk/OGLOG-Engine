#include "linear_algebra.h";
#include <IOStream>

vec3 operator+(const vec3& val1, const vec3& val2)
{
    return {
        val1.entries[0] + val2.entries[0],
        val1.entries[1] + val2.entries[1],
        val1.entries[2] + val2.entries[2]
    };
}

vec3 operator-(const vec3& val1, const vec3& val2)
{
    return {
        val1.entries[0] - val2.entries[0],
        val1.entries[1] - val2.entries[1],
        val1.entries[2] - val2.entries[2]
    };
}


mat4 create_matrix_transform(vec3 translation)
{
    mat4 matrix;

    // for (int i = 0; i < 12; i++)    { matrix.entries[i] = 0.0f; }

    // for (int i = 0; i < 3; i++)     { matrix.entries[i + 12] = translation.entries[i]; }

    // for (int i = 0; i < 16; i += 5) { matrix.entries[i] = 1; }
    matrix.entries[0]  = 1.0f;
    matrix.entries[1]  = 0.0f;
    matrix.entries[2]  = 0.0f;
    matrix.entries[3]  = 0.0f;
    
    matrix.entries[4]  = 0.0f;
    matrix.entries[5]  = 1.0f;
    matrix.entries[6]  = 0.0f;
    matrix.entries[7]  = 0.0f;
    
    matrix.entries[8]  = 0.0f;
    matrix.entries[9]  = 0.0f;
    matrix.entries[10] = 1.0f;
    matrix.entries[11] = 0.0f;
    
    matrix.entries[12] = translation.entries[0];
    matrix.entries[13] = translation.entries[1];
    matrix.entries[14] = translation.entries[2];
    matrix.entries[15] = 1.0f;

    return matrix;
}

mat4 create_z_rotation(float angle)
{
    angle = angle * PI / 180.f;

    mat4 matrix;

    float c = cosf(angle);
    float s = sinf(angle);

    matrix.entries[0]  = c;
    matrix.entries[1]  = s;
    matrix.entries[2]  = 0.0f;
    matrix.entries[3]  = 0.0f;

    matrix.entries[4]  = -s;
    matrix.entries[5]  = c;
    matrix.entries[6]  = 0.0f;
    matrix.entries[7]  = 0.0f;
    
    matrix.entries[8]  = 0.0f;
    matrix.entries[9]  = 0.0f;
    matrix.entries[10] = 1.0f;
    matrix.entries[11] = 0.0f;
    
    matrix.entries[12] = 0.0f;
    matrix.entries[13] = 0.0f;
    matrix.entries[14] = 0.0f;
    matrix.entries[15] = 1.0f;
    
    return matrix;
}

mat4 create_model_transform(vec3 position, float angle)
{
    angle = angle * PI / 180.f;

    mat4 matrix;

    float c = cosf(angle);
    float s = sinf(angle);

    matrix.entries[0]  = c;
    matrix.entries[1]  = s;
    matrix.entries[2]  = 0.0f;
    matrix.entries[3]  = 0.0f;

    matrix.entries[4]  = -s;
    matrix.entries[5]  = c;
    matrix.entries[6]  = 0.0f;
    matrix.entries[7]  = 0.0f;
    
    matrix.entries[8]  = 0.0f;
    matrix.entries[9]  = 0.0f;
    matrix.entries[10] = 1.0f;
    matrix.entries[11] = 0.0f;
    
    matrix.entries[12] = position.entries[0];
    matrix.entries[13] = position.entries[1];
    matrix.entries[14] = position.entries[2];
    matrix.entries[15] = 1.0f;
    
    return matrix;
}

mat4 create_look_at(vec3 from, vec3 to)
{
    vec3 global_up = {0.0f, 1.0f, 0.0f};
    vec3 forward = normalize(to - from);

    vec3 right =    normalize(cross(forward, global_up));
    vec3 up =       normalize(cross(right, forward));

    mat4 matrix;

    matrix.entries[0]  = right.entries[0];
    matrix.entries[1]  = up.entries[0];
    matrix.entries[2]  = -forward.entries[0];
    matrix.entries[3]  = 0.0f;

    matrix.entries[4]  = right.entries[1];
    matrix.entries[5]  = up.entries[1];
    matrix.entries[6]  = -forward.entries[1];
    matrix.entries[7]  = 0.0f;
    
    matrix.entries[8]  = right.entries[2];
    matrix.entries[9]  = up.entries[2];
    matrix.entries[10] = -forward.entries[2];
    matrix.entries[11] = 0.0f;
    
    matrix.entries[12] = -dot(right, from);
    matrix.entries[13] = -dot(up, from);
    matrix.entries[14] = dot(forward, from);
    matrix.entries[15] = 1.0f;

    return matrix;
}

float dot(vec3 u, vec3 v)
{
    return u.entries[0] * v.entries[0] + u.entries[1] * v.entries[1] + u.entries[2] * v.entries[2];
}


vec3 normalize(vec3 v)
{
    float magnitude = sqrtf(dot(v, v));

    return {
        v.entries[0] / magnitude,
        v.entries[1] / magnitude,
        v.entries[2] / magnitude
    };
}

vec3 cross(vec3 forward, vec3 upward)
{
    return {
          forward.entries[1] * upward.entries[2] - forward.entries[2] * upward.entries[1],
        -(forward.entries[0] * upward.entries[2] - forward.entries[2] * upward.entries[0]),
          forward.entries[0] * upward.entries[1] - forward.entries[1] * upward.entries[0]
    };
}