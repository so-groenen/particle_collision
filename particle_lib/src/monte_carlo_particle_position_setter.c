#include "particle_lib/monte_carlo_particle_position_setter.h"

 


void MonteCarloSetProposalPosition(Particle* particle, const Box* box, float deltaR)
{
    const float r                =  particle->radius;
    const unsigned int BoxWidth  =  (box->xRight  - box->xLeft);
    const unsigned int BoxHeight =  (box->yBottom - box->yTop);

    const float x_mc  =  (box->xLeft + r) + (BoxWidth  - 2*r)*(float)rand()/RAND_MAX;
    const float y_mc  =  (box->yTop  + r) + (BoxHeight - 2*r)*(float)rand()/RAND_MAX;

    particle->pos.x = x_mc;
    particle->pos.y = y_mc; 
} 



bool MonteCarloShouldAccept(size_t index, const Particle* particle, size_t N_part)
{
    for (size_t n = 0; n < N_part; n++)
    {
        if (n != index && CheckCollisionCircles(particle[n].pos, (1.01)*particle[n].radius, particle[index].pos, (1.01)*particle[index].radius))
        {
            return FAIL;
        }
    }
    return SUCCESS;
}

void MonteCarloUpdatePosition(size_t index, Particle* particles, size_t N_part, const Box* box, float deltaR)
{
    uint64_t attempt = 0;
    do
    {
        MonteCarloSetProposalPosition(&particles[index], box, deltaR);
        attempt++;   
    } while (MonteCarloShouldAccept(index, particles, N_part) == FAIL);
    

    if(DEBUG) printf("MONTECARLO: (particle %zu) attempt = %llu\n", index, attempt);
}
