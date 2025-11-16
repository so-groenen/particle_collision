#ifndef MONTE_CARLO_REMOVER_H
#define MONTE_CARLO_REMOVER_H

#define SUCCESS true
#define FAIL false
#define DEBUG false

#include "particle.h"


void MonteCarlo_setProposalPosition(particle_t* particle, const Box_t* box, float deltaR);
bool MonteCarlo_getAcceptance(size_t index, particle_t* particle, size_t N_part);
void MonteCarlo_UpdateParticlePosition(size_t index, particle_t* particles, size_t N_part, const Box_t* box, float deltaR);


#endif