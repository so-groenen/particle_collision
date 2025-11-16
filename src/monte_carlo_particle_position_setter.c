#include "monte_carlo_particle_position_setter.h"


// void MonteCarloSetProposalPosition(Particle* particle, const Box* box, float deltaR)
// {
//     float r     =  particle->radius;
//     float theta =  PI*(float)rand()/RAND_MAX;
//     float x_mc  =  particle->pos.x + deltaR*cos(theta);
//     float y_mc  =  particle->pos.y + deltaR*sin(theta);
//     if ((x_mc) + r >= box->xRight)
//     {
//         x_mc -= ( (x_mc + r) - box->xRight);
//     }
//     if ((x_mc) - r <= box->xLeft)
//     {
//         x_mc += (box->xLeft - (x_mc - r));
//     }
//     if ((y_mc) - r <= box->yTop)
//     {
//         y_mc += (box->yTop - (y_mc - r));
//     }
//     if ((y_mc) + r >= box->yBottom)
//     {
//         y_mc -= ( (y_mc + r) - box->yBottom);
//     }
//     particle->pos.x = x_mc;
//     particle->pos.y = y_mc; 
// } 



void MonteCarloSetProposalPosition(Particle* particle, const Box* box, float deltaR)
{
    float r                =  particle->radius;
    unsigned int BoxWidth  =  (box->xRight  - box->xLeft);
    unsigned int BoxHeight =  (box->yBottom - box->yTop);
    float x_mc  =  (box->xLeft + r) + (BoxWidth  - 2*r)*(float)rand()/RAND_MAX;
    float y_mc  =  (box->yTop  + r) + (BoxHeight - 2*r)*(float)rand()/RAND_MAX;

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
    // while (MonteCarloShouldAccept(index, particles, N_part) == FAIL)
    // {
    //     MonteCarloSetProposalPosition(&particles[index], box, deltaR);
    //     attempt++;   
    // }
    do
    {
        MonteCarloSetProposalPosition(&particles[index], box, deltaR);
        attempt++;   
    } while (MonteCarloShouldAccept(index, particles, N_part) == FAIL);
    

    if(DEBUG) printf("MONTECARLO: (particle %zu) attempt = %lu\n", index, attempt);
}
