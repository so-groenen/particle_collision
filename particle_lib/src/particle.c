#include "particle_lib/particle.h"

int BoxGetWidth(const Box* box)
{
    return box->xRight - box->xLeft; 
}
int BoxGetHeigth(const Box* box)
{
    return abs(box->yTop - box->yBottom); 
}
Vector2 BoxGetUpperLeft(const Box* box)
{
    return (Vector2){box->xLeft, box->yTop}; 
}
Vector2 BoxGetCenter(const Box* box)
{
    return (Vector2){box->xLeft + BoxGetWidth(box)/2.f, box->yTop + BoxGetHeigth(box)/2.f}; 
}
void BoxRender(const Box* box, float thickness, Color color)
{
    DrawLineEx((Vector2){box->xLeft,  box->yBottom}, (Vector2){box->xLeft,  box->yTop},    thickness, color);
    DrawLineEx((Vector2){box->xRight, box->yBottom}, (Vector2){box->xRight, box->yTop},    thickness, color);
    DrawLineEx((Vector2){box->xLeft,  box->yBottom}, (Vector2){box->xRight, box->yBottom}, thickness, color);
    DrawLineEx((Vector2){box->xLeft,  box->yTop},    (Vector2){box->xRight, box->yTop},    thickness, color);
}

void ParticleSet(Particle* particle, Vector2 position, Vector2 velocity, float radius, float mass)
{
    (*particle) = (Particle)
    {
        .pos    = position,
        .vel    = velocity,
        .radius = radius,
        .mass   = mass
    };
}
void ParticleLogInfo(const Particle* p, size_t N)
{
    for (size_t i = 0; i < N; i++)
    {
        printf("position = (%.1f, %.1f)\n", p[i].pos.x, p[i].pos.y);
    }
}
RenderTexture2D ParticleCreateRenderTexture(float radius, Color particle_color)
{
    int rendererTextureLen   = 2*(int)radius;
    RenderTexture2D renderer = LoadRenderTexture(rendererTextureLen, rendererTextureLen);

    BeginTextureMode(renderer);
    ClearBackground(RAYWHITE);
        DrawCircle(radius, radius, radius, particle_color);
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
    float r            = particle->radius;
    bool outsideRight  = (particle->pos.x + r >= box->xRight  && particle->vel.x > 0);
    bool outsideLeft   = (particle->pos.x - r <= box->xLeft   && particle->vel.x < 0);
    bool outsideTop    = (particle->pos.y - r <= box->yTop    && particle->vel.y < 0);
    bool outsideBottom = (particle->pos.y + r >= box->yBottom && particle->vel.y > 0);

    if (outsideRight || outsideLeft)
    {
        particle->vel.x *= -1;
    }
    if (outsideTop || outsideBottom)
    {
        particle->vel.y *= -1;
    }
}
void ParticleCheckCollisions(Particle* particles, size_t N)
{
    float distSqr    = 0.f;
    float m_i        = 0.f;
    float m_j        = 0.f;
    float m_transfer = 0.f;
    Vector2 V_ij     = {0};
    Vector2 R_ij     = {0};
    Vector2 deltaV_i = {0};
    Vector2 momentum_transfer = {0};
    float  VdotR = 0.f;
    for (size_t i = 0; i < N-1; i++)
    {
        for (size_t j = i+1; j < N; j++)
        {
            if(CheckCollisionCircles(particles[i].pos, particles[i].radius, particles[j].pos, particles[j].radius))
            {
                R_ij    = VectorGetRelative(&particles[i].pos, &particles[j].pos);
                V_ij    = VectorGetRelative(&particles[i].vel, &particles[j].vel);
                VdotR   = VectorGetDotProd(&R_ij, &V_ij);
                distSqr = VectorGetDotProd(&R_ij, &R_ij);
                if(VdotR < 0)
                {
                    m_i        = particles[i].mass;
                    m_j        = particles[j].mass;
                    m_transfer = (2*m_i*m_j)/(m_i+m_j);

                    momentum_transfer = VectorScale( m_transfer*( VdotR/distSqr), &R_ij );
                    
                    VectorScaleAndAddInPlace(&particles[i].vel, &momentum_transfer, + (1/m_i));
                    VectorScaleAndAddInPlace(&particles[j].vel, &momentum_transfer, - (1/m_j));
                }
            }
        }
    }
}
void ParticleRemoveCenterOfMass(Particle* particles, size_t N)
{
    float px_CM     = 0.f;
    float py_CM     = 0.f;
    float totalMass = 0.f;

    for (size_t i = 0; i < N; i++)
    {
        px_CM     += particles[i].vel.x*particles[i].mass;
        py_CM     += particles[i].vel.y*particles[i].mass;
        totalMass += particles[i].mass;
    }
    for (size_t i = 0; i < N; i++)
    {
        particles[i].vel.x -= px_CM/(totalMass);
        particles[i].vel.y -= py_CM/(totalMass);
    }
}

float ParticleGetSpeed(const Particle* particle)
{
    return sqrt(VectorGetDotProd(&particle->vel, &particle->vel));
}
Vector2 ParticleGetCenter(const Particle* particle)
{
    return (Vector2){particle->pos.x - particle->radius, particle->pos.y - particle->radius};
}

void ParticleUpdateInfo(float *minSpeed, float *maxSpeed, float *averageSpeed, const Particle *particles, size_t N)
{
    float sum   = 0.f;
    float min   = __FLT_MAX__;
    float max   = 0.f;
    float speed = 0.f;
    
    for (size_t i = 0; i < N; i++)
    {
        speed = ParticleGetSpeed(&particles[i]);
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
    (*minSpeed)     = min;
    (*maxSpeed)     = max;
    (*averageSpeed) = (sum/(float)N);
}
