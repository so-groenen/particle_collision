#ifndef MONTE_CARLO_REMOVER_H
#define MONTE_CARLO_REMOVER_H



#define MONTECARLO_MAX_ATTEMPT (unsigned int)5e6

#include "particle.h"


void MonteCarloSetProposalPosition(Particle* particle, const Box* box, float deltaR);
bool MonteCarloShouldAccept(size_t index, const Particle* particle, size_t N_part);
bool MonteCarloUpdatePosition(size_t index, Particle* particles, size_t N_part, const Box* box, float deltaR);


#endif