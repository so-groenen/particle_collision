#ifndef VECTOR_ALGEBRA_H
#define VECTOR_ALGEBRA_H


#include "raylib.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

Vector2 getRelativeVector(const Vector2* v1, const Vector2* v2);
float dotProd(const Vector2* v1, const Vector2* v2);
Vector2 scaleVec(float s, const Vector2* vec);
void addVec(Vector2* v1, const Vector2* v2);
void subVec(Vector2* v1, const Vector2* v2);
void addVecFactor(Vector2* v1, const Vector2* v2, float s);


#endif