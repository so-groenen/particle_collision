#include "particle_lib/particle.h"
 
typedef struct
{
    float     min;
    float     max;
    uint64_t* binValues;
    size_t    Nbins;
} Histogram;

void HistogramCompute(const Particle* particles, size_t n_part, Histogram* histogram);
bool HistogramInit(Histogram* histogram, size_t Nbins, float min, float max);
void HistogramRelease(Histogram* histogram);
void HistogramFillBins(Rectangle* bin, uint64_t binValue, float scalingFactor, float MaxHeight);

 