#include "particle.h"




void ParticleSet(Particle* particle, Vector2 velocity, float radius, float mass)
{
    (*particle) = (Particle){.vel    = velocity,
                               .radius = radius,
                               .mass   = mass};
}
void ParticleLogInfo(const Particle* p, size_t N)
{
    for (size_t i = 0; i < N; i++)
    {
        printf("position = (%.1f, %.1f)\n", p[i].pos.x, p[i].pos.y);
    }
}
RenderTexture2D ParticleCreateRenderTexture(float radius)
{
    int RendererTextureLen   = 2*(int)radius;

    RenderTexture2D renderer = LoadRenderTexture(RendererTextureLen, RendererTextureLen);

    BeginTextureMode(renderer);
    ClearBackground(RAYWHITE);
        DrawCircle(radius, radius, radius, BLACK);
    EndTextureMode();
    return renderer;
}
void ParticleMove(Particle* particle, float dt)
{
    particle->pos.x += particle->vel.x*dt;
    particle->pos.y += particle->vel.y*dt;
}
void ParticleCheckBoundary(Particle* particle, const Box* box)
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
void ParticleCheckCollisions(Particle* particles, size_t N)
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
void ParticleRemoveCenterOfMass(Particle* particles, size_t N)
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

float ParticleGetVelocity(const Particle* particle)
{
    return  sqrt(dotProd(&particle->vel, &particle->vel));
}


void ParticleUpdateInfo(float* MinSpeed, float* MaxSpeed, float* AverageSpeed, const Particle* particles, size_t N)
{
    float sum = 0;
    float min = 1E6;
    float max = 0;
    float speed = 0;
    for (size_t i = 0; i < N; i++)
    {
        speed = ParticleGetVelocity(&particles[i]);
        if (speed < min)
        {
            min = speed;
        }
        if (speed > max)
        {
            max = speed;
        }
        sum += speed;
    }
    (*MinSpeed)     = min;
    (*MaxSpeed)     = max;
    (*AverageSpeed) = (sum/(float)N);
}
