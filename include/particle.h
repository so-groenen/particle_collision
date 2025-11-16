#ifndef PARTICLE_H
#define PARTICLE_H

#include "vectorAlgebra.h"



typedef struct 
{
    Vector2 pos;
    Vector2 vel;
    float radius;
    float mass;
}particle_t; 

typedef struct 
{
    int xLeft;
    int xRight;
    int yTop;
    int yBottom; 
} Box_t;



void initParticle(particle_t* particle, Vector2 velocity, float radius, float mass);
void checkPositions(const particle_t* p, size_t N);
RenderTexture2D createParticleRenderer(float radius);
void ParticleMove(particle_t* particle, float dt);
void ParticleCheckBoundary(particle_t* particle, const Box_t* box);
void ParticleCheckCollisions(particle_t* particles, size_t N);
void ParticleRemoveCenterOfMass(particle_t* particles, size_t N);
float getSpeed(const particle_t* particle);


#endif