#include "particle.h"




void initParticle(particle_t* particle, Vector2 velocity, float radius, float mass)
{
    (*particle) = (particle_t){.vel    = velocity,
                               .radius = radius,
                               .mass   = mass};
}
void checkPositions(const particle_t* p, size_t N)
{
    for (size_t i = 0; i < N; i++)
    {
        printf("position = (%.1f, %.1f)\n", p[i].pos.x, p[i].pos.y);
    }
}
RenderTexture2D createParticleRenderer(float radius)
{
    int RendererTextureLen    = 2*(int)radius;

    RenderTexture2D renderer = LoadRenderTexture(RendererTextureLen, RendererTextureLen);

    BeginTextureMode(renderer);
    ClearBackground(RAYWHITE);
        DrawCircle(radius, radius, radius, BLACK);
    EndTextureMode();
    return renderer;
}
void ParticleMove(particle_t* particle, float dt)
{
    particle->pos.x += particle->vel.x*dt;
    particle->pos.y += particle->vel.y*dt;
}
void ParticleCheckBoundary(particle_t* particle, const Box_t* box)
{
    float r = particle->radius;
    bool OutsideRight  =  (particle->pos.x + r >= box->xRight  && particle->vel.x > 0);
    bool OutsideLeft   =  (particle->pos.x - r <= box->xLeft   && particle->vel.x < 0);

    bool OutsideTop    =  (particle->pos.y - r <= box->yTop    && particle->vel.y < 0);
    bool OutsideBottom =  (particle->pos.y + r >= box->yBottom && particle->vel.y > 0);

    if (OutsideRight || OutsideLeft)
    {
        particle->vel.x *= -1;
    }
    if (OutsideTop || OutsideBottom)
    {
        particle->vel.y *= -1;
    }
}
void ParticleCheckCollisions(particle_t* particles, size_t N)
{
    float distSqr;
    float m_i;
    float m_j;
    float m_transfer;
    Vector2 V_ij;
    Vector2 deltaV_i;
    Vector2 momentum_transfer;
    Vector2 R_ij;
    float   VdotR;
    
    for (size_t i = 0; i < N-1; i++)
    {
        for (size_t j = i+1; j < N; j++)
        {
            if(CheckCollisionCircles(particles[i].pos, particles[i].radius, particles[j].pos, particles[j].radius))
            {
                R_ij    = getRelativeVector(&particles[i].pos, &particles[j].pos);
                V_ij    = getRelativeVector(&particles[i].vel, &particles[j].vel);
                VdotR   = dotProd(&R_ij, &V_ij);
                distSqr = dotProd(&R_ij, &R_ij);
                if(VdotR < 0)
                {
                    m_i        = particles[i].mass;
                    m_j        = particles[j].mass;
                    m_transfer = (2*m_i*m_j)/(m_i+m_j);

                    momentum_transfer = scaleVec( m_transfer*( VdotR/distSqr), &R_ij );
                    
                    addVecFactor(&particles[i].vel, &momentum_transfer, +(1/m_i));
                    addVecFactor(&particles[j].vel, &momentum_transfer, -(1/m_j));
                }
            }
        }
    }
}
void ParticleRemoveCenterOfMass(particle_t* particles, size_t N)
{
    float px_CM = 0;
    float py_CM = 0;
    float TotalMass = 0;
    for (size_t i = 0; i < N; i++)
    {
        px_CM += particles[i].vel.x*particles[i].mass;
        py_CM += particles[i].vel.y*particles[i].mass;
        TotalMass += particles[i].mass;
    }
    for (size_t i = 0; i < N; i++)
    {
        particles[i].vel.x -= px_CM/(TotalMass);
        particles[i].vel.y -= py_CM/(TotalMass);
    }
}

float getSpeed(const particle_t* particle)
{
    return  sqrt(dotProd(&particle->vel, &particle->vel));
}

