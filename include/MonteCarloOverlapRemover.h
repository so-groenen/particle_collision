#ifndef MONTE_CARLO_REMOVER_H
#define MONTE_CARLO_REMOVER_H

#define SUCCESS true
#define FAIL false
#define DEBUG false

#include "particle.h"


void MonteCarlo_setProposalPosition(Particle* particle, const Box* box, float deltaR);
bool MonteCarlo_getAcceptance(size_t index, Particle* particle, size_t N_part);
void MonteCarlo_UpdateParticlePosition(size_t index, Particle* particles, size_t N_part, const Box* box, float deltaR);


#endif