#include "particle_lib/vector_algebra.h"


Vector2 VectorGetRelative(const Vector2* v1, const Vector2* v2)
{
    return (Vector2){(v2->x - v1->x), (v2->y - v1->y)};
}
float VectorGetDotProd(const Vector2* v1, const Vector2* v2)
{
    float X = (v1->x)*(v2->x);
    float Y = (v1->y)*(v2->y);
    return X+Y;
}
Vector2 VectorScale(float s, const Vector2* vec)
{
    return (Vector2){s * vec->x, s * vec->y};
}
void VectorAddInPlace(Vector2* v1, const Vector2* v2)
{
    v1->x += v2->x;
    v1->y += v2->y;
}
void VectorRemoveInPlace(Vector2* v1, const Vector2* v2)
{
    v1->x -= v2->x;
    v1->y -= v2->y;
}
// inline Vector2 VectorSubtract(const Vector2* lhs, const Vector2* rhs)
// {
//     return VectorGetRelative(lhs, rhs);// (Vector2){lhs->x - rhs->x, lhs->y - rhs->y};
// }
void VectorScaleAndAddInPlace(Vector2* v1, const Vector2* v2, float s)
{
    v1->x += s*v2->x;
    v1->y += s*v2->y;
}