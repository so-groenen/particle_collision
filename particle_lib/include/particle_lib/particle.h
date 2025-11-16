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
int BoxGetWidth(const Box* box);
int BoxGetHeigth(const Box* box);
Vector2 BoxGetUpperLeft(const Box* box);
void BoxRender(const Box* box, float thickness, Color color);

Vector2 ParticleGetCenter(const Particle* particle);
void ParticleUpdateInfo(float* minSpeed, float* maxSpeed, float* averageSpeed, const Particle* particles, size_t N);
void ParticleSet(Particle* particle, Vector2 position, Vector2 velocity, float radius, float mass);
void ParticleLogInfo(const Particle* p, size_t N);
RenderTexture2D ParticleCreateRenderTexture(float radius, Color particle_color);
void ParticleMove(Particle* particle, float dt);
void ParticleCheckBoundary(Particle* particle, const Box* box);
void ParticleCheckCollisions(Particle* particles, size_t N);
void ParticleRemoveCenterOfMass(Particle* particles, size_t N);
float ParticleGetSpeed(const Particle* particle);


#endif