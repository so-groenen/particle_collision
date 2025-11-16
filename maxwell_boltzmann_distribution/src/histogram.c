#include "histogram.h"
 

void HistogramCompute(const Particle* particles, size_t n_part, Histogram* histogram)
{
    float Binwidth = (histogram->max - histogram->min) / histogram->Nbins;
    size_t bin_number;
    for (size_t i = 0; i < n_part; i++)
    {
        bin_number = (size_t)(  ParticleGetSpeed(&particles[i]) / Binwidth  );
        if (bin_number < histogram->Nbins)
        {
            histogram->binValues[bin_number] += 1;
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



void HistogramFillBins(Rectangle* bin, uint64_t binValue, float scalingFactor, float MaxHeight)
{
    float oldHeight = bin->height;
    float NewHeight = (scalingFactor * binValue);

    if(NewHeight < 10.0f)
    {
        NewHeight = 10.0f;
    }
    if(NewHeight > MaxHeight)
    {
        NewHeight = MaxHeight;
    }
    float deltaH = NewHeight - oldHeight;
    bin->height  = NewHeight;
    bin->y      -= deltaH;
}

