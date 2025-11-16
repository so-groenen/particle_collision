#include "vectorAlgebra.h"


Vector2 getRelativeVector(const Vector2* v1, const Vector2* v2)
{
    Vector2 v12;
    v12.x = v2->x - v1->x;
    v12.y = v2->y - v1->y;
    return v12;
}
float dotProd(const Vector2* v1, const Vector2* v2)
{
    float X = (v1->x)*(v2->x);
    float Y = (v1->y)*(v2->y);
    return X+Y;
}
Vector2 scaleVec(float s, const Vector2* vec)
{
    Vector2 svec = {s * vec->x, s * vec->y};
    return svec;
}
void addVec(Vector2* v1, const Vector2* v2)
{
    v1->x += v2->x;
    v1->y += v2->y;
}
void subVec(Vector2* v1, const Vector2* v2)
{
    v1->x -= v2->x;
    v1->y -= v2->y;
}
void addVecFactor(Vector2* v1, const Vector2* v2, float s)
{
    v1->x += s*v2->x;
    v1->y += s*v2->y;
}