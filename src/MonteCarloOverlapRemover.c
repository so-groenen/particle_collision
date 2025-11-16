#include "MonteCarloOverlapRemover.h"


// void MonteCarlo_setProposalPosition(particle_t* particle, const Box_t* box, float deltaR)
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



void MonteCarlo_setProposalPosition(particle_t* particle, const Box_t* box, float deltaR)
{
    float r                =  particle->radius;
    unsigned int BoxWidth  =  (box->xRight  - box->xLeft);
    unsigned int BoxHeight =  (box->yBottom - box->yTop);
    float x_mc  =  (box->xLeft + r) + (BoxWidth  - 2*r)*(float)rand()/RAND_MAX;
    float y_mc  =  (box->yTop  + r) + (BoxHeight - 2*r)*(float)rand()/RAND_MAX;

    particle->pos.x = x_mc;
    particle->pos.y = y_mc; 
} 



bool MonteCarlo_getAcceptance(size_t index, particle_t* particle, size_t N_part)
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

void MonteCarlo_UpdateParticlePosition(size_t index, particle_t* particles, size_t N_part, const Box_t* box, float deltaR)
{
    uint64_t attempt = 0;
    // while (MonteCarlo_getAcceptance(index, particles, N_part) == FAIL)
    // {
    //     MonteCarlo_setProposalPosition(&particles[index], box, deltaR);
    //     attempt++;   
    // }
    do
    {
        MonteCarlo_setProposalPosition(&particles[index], box, deltaR);
        attempt++;   
    } while (MonteCarlo_getAcceptance(index, particles, N_part) == FAIL);
    

    if(DEBUG) printf("MONTECARLO: (particle %zu) attempt = %lu\n", index, attempt);
}
