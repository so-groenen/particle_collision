#ifndef VECTOR_ALGEBRA_H
#define VECTOR_ALGEBRA_H


#include "raylib.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

Vector2 VectorGetRelative(const Vector2* v1, const Vector2* v2);
float VectorGetDotProd(const Vector2* v1, const Vector2* v2);
Vector2 VectorScale(float s, const Vector2* vec);
void VectorAddInPlace(Vector2* v1, const Vector2* v2);
void subVec(Vector2* v1, const Vector2* v2);
void VectorScaleAndAddInPlace(Vector2* v1, const Vector2* v2, float s);


#endif