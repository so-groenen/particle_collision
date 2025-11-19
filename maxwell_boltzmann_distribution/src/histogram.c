#include "histogram.h"
 
#define HISTOGRAM_MIN_HEIGHT 10.0f


void HistogramCompute(const Particle* particles, size_t n_part, Histogram* histogram)
{
    float binWidth   = (histogram->max - histogram->min) / histogram->Nbins;
    size_t binNumber = 0;
    for (size_t i = 0; i < n_part; i++)
    {
        binNumber = (size_t)(ParticleGetSpeed(&particles[i]) / binWidth);
        if (binNumber < histogram->Nbins)
        {
            histogram->binValues[binNumber] += 1;
        }
        else
        {
            histogram->binValues[(histogram->Nbins-1)] += 1; // everything greater than "max" is put in the last bin...
        }
    }
}

bool HistogramInit(Histogram* histogram, size_t Nbins, float min, float max)
{
    uint64_t* data = calloc(Nbins, sizeof(uint64_t));
    if (data == NULL)
    {
        return false;
    }
    
    histogram->binValues = data;
    histogram->max       = max;
    histogram->max       = max;
    histogram->Nbins     = Nbins;    
    return true;
}

void HistogramRelease(Histogram* histogram)
{
    free(histogram->binValues);
    histogram->binValues = NULL;
}

void HistogramFillBins(Rectangle* bin, uint64_t binValue, float scalingFactor, float maxHeight)
{
    float oldHeight = bin->height;
    float newHeight = (scalingFactor * binValue);

    if(newHeight < HISTOGRAM_MIN_HEIGHT)
    {
        newHeight = HISTOGRAM_MIN_HEIGHT;
    }
    if(newHeight > maxHeight)
    {
        newHeight = maxHeight;
    }
    float deltaH = newHeight - oldHeight;
    bin->height  = newHeight;
    bin->y      -= deltaH;
}

