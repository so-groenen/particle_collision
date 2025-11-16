#ifndef MONTE_CARLO_REMOVER_H
#define MONTE_CARLO_REMOVER_H

#define SUCCESS true
#define FAIL false
#define DEBUG false

#include "particle.h"


void MonteCarloSetProposalPosition(Particle* particle, const Box* box, float deltaR);
bool MonteCarloShouldAccept(size_t index, const Particle* particle, size_t N_part);
void MonteCarloUpdatePosition(size_t index, Particle* particles, size_t N_part, const Box* box, float deltaR);


#endif