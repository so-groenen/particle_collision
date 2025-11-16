#ifndef PARTICLE_H
#define PARTICLE_H

#include "vector_algebra.h"



typedef struct 
{
    Vector2 pos;
    Vector2 vel;
    float radius;
    float mass;
}Particle; 

typedef struct 
{
    int xLeft;
    int xRight;
    int yTop;
    int yBottom; 
} Box;


void ParticleUpdateInfo(float* MinSpeed, float* MaxSpeed, float* AverageSpeed, const Particle* particles, size_t N);
void ParticleSet(Particle* particle, Vector2 velocity, float radius, float mass);
void ParticleLogInfo(const Particle* p, size_t N);
RenderTexture2D ParticleCreateRenderTexture(float radius);
void ParticleMove(Particle* particle, float dt);
void ParticleCheckBoundary(Particle* particle, const Box* box);
void ParticleCheckCollisions(Particle* particles, size_t N);
void ParticleRemoveCenterOfMass(Particle* particles, size_t N);
float ParticleGetVelocity(const Particle* particle);


#endif