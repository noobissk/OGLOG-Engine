#pragma once
#include <vector>
#include <cmath>

struct Vector2 {
public:
    int x;
    int y;
    Vector2() = default;
    Vector2(int _x, int _y) : x(_x), y(_y) {}
};

inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
    // Clamp t between 0 and 1 (optional but often useful)
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    int x = static_cast<int>(std::round(a.x + (b.x - a.x) * t));
    int y = static_cast<int>(std::round(a.y + (b.y - a.y) * t));
    return Vector2(x, y);
}

inline Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    float u = 1.0f - t;
    float x = u*u * p0.x + 2*u*t * p1.x + t*t * p2.x;
    float y = u*u * p0.y + 2*u*t * p1.y + t*t * p2.y;

    return Vector2(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)));
}