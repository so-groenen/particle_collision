#include "particle_lib/particle.h"
#include "particle_lib/monte_carlo_particle_position_setter.h"
#include <time.h>

#define N_part 2000UL

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


int main(void)
{
    const uint64_t windowWidth  = 860;
    const uint64_t windoWheight = 480;
    InitWindow(windowWidth, windoWheight, "Maxwell-Boltzmann distribution");
    
    SetRandomSeed(123);
    srand(time(NULL));

    // <-- Box for particles --> 
    Box box = 
    {
        .xLeft   =   windowWidth*(0.05),       
        .xRight  =   (0.55)*windowWidth,
        .yTop    =   windoWheight*(0.05),
        .yBottom =   (0.95)*windoWheight
    };

    RenderTexture2D boxTexture = LoadRenderTexture(windowWidth, windoWheight);
    
    const float thickness = 1;
    BeginTextureMode(boxTexture);
        ClearBackground(RAYWHITE);
        DrawLineEx((Vector2){box.xLeft,  box.yBottom}, (Vector2){box.xLeft,  box.yTop},    thickness, BLACK);
        DrawLineEx((Vector2){box.xRight, box.yBottom}, (Vector2){box.xRight, box.yTop},    thickness, BLACK);
        DrawLineEx((Vector2){box.xLeft,  box.yBottom}, (Vector2){box.xRight, box.yBottom}, thickness, BLACK);
        DrawLineEx((Vector2){box.xLeft,  box.yTop},    (Vector2){box.xRight, box.yTop},    thickness, BLACK);
    EndTextureMode();


    // < -- Particles --> 
    const float radius   = 2;
    const float velocity = 20; //  pixel / secondes
    const float mass     = 1;
    Vector2 pos          = {0};
    Vector2 vel          = {0};
    
    Particle particle[N_part];

    for (size_t i = 0; i < N_part; i++)
    {
        vel = (Vector2){GetRandomValue(-velocity, velocity), GetRandomValue(-velocity, velocity)};
        ParticleSet(&particle[i], vel, radius, mass);
    }
    ParticleRemoveCenterOfMass(particle, N_part);
 
    for (size_t n = 0; n < N_part; n++)
    {
        size_t ParticleNumberToCheck = n;
        MonteCarloUpdatePosition(n, particle, ParticleNumberToCheck, &box, radius);
    }

    RenderTexture2D particleRenderTex = ParticleCreateRenderTexture(particle[1].radius);
 

    Histogram histogram = {0};
    const float min     = 0;
    const float max     = 45;
    const size_t Nbins  = 20;
    if(!HistogramInit(&histogram, Nbins, min, max))
    {
        perror("Could not init Histogram: allocation fail");
        return EXIT_FAILURE;
    }

    //Histogram Rectangle;
    float xStart           = (0.6)*windowWidth;
    float yStart           = (0.9)*windoWheight;
    float binRecWidth      = 16.0f;
    float binRecHeightIinit = 32.0f;

    // float scalingFactor    = 3; // scaling between binValues --> binRectangle height on the screen.
    float MaxHeight        = (0.8)*GetScreenHeight();
    float scalingFactor    =  MaxHeight/(0.15*N_part);


    Texture2D nPatchTexture = LoadTexture("ninepatch_button.png");
    NPatchInfo v3PatchInfo  = { (Rectangle){ 0.0f, 192.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH}; // from raylib
    Vector2 origin          = {0.0f, 0.0f}; // for rotation, but rotation is kept at 0.

    Rectangle bins[Nbins];
    for (size_t n = 0; n < Nbins; n++)
    {
        bins[n] = (Rectangle){xStart + n*binRecWidth, yStart, binRecWidth, binRecHeightIinit};
    }
    
    // <-- Simulation Start -->
    uint64_t targetFPS    = 100;
    uint64_t frameCounter = 0;

    float dt =  1.0f / (float)targetFPS;

    float minSpeed     = 0.f;
    float maxSpeed     = 0.f;
    float averageSpeed = 0.f;

    // SetTargetFPS(targetFPS);
    while (!WindowShouldClose())
    {
        
        // dt = GetFrameTime();
    
        // <-- particle updates --> //
        ParticleCheckCollisions(particle, N_part);
        for (size_t i = 0; i < N_part; i++)
        {
            ParticleCheckBoundary(&particle[i], &box); //Implicit Euler: First update velocity, then update positions accordingly.
            ParticleMove(&particle[i], dt);
        }

        if(frameCounter%10 == 0 )
        {
            ParticleUpdateInfo(&minSpeed, &maxSpeed, &averageSpeed, particle, N_part);
            HistogramCompute(particle, N_part, &histogram);
            for (size_t n = 0; n < Nbins; n++)
            {
                HistogramFillBins(&bins[n], histogram.binValues[n], scalingFactor, MaxHeight);
            }
            memset(histogram.binValues, 0, Nbins*sizeof(uint64_t));

            if(frameCounter == UINT64_MAX - 1)
            {
                frameCounter = 0;
            }
        }


        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTextureV(boxTexture.texture, (Vector2){0, 0}, WHITE); // when drawing the particles, the position is the edge of the texture, not the center. We can offset it here, but also at the particle level
            for (size_t i = 0; i < N_part; i++)
            {
                DrawTextureV(particleRenderTex.texture, (Vector2){particle[i].pos.x - particle[i].radius, particle[i].pos.y - particle[i].radius}, WHITE);
            }
            for (size_t n = 0; n < Nbins; n++)
            {
                DrawTextureNPatch(nPatchTexture, v3PatchInfo, bins[n], origin, 0.0f, WHITE);
            }
            DrawFPS(GetScreenWidth()*(0.05), GetScreenHeight()*(0.0125));
            DrawText(TextFormat("SpeedInfo: Min:%.2f, Max:%.2f, avg:%.2f", minSpeed, maxSpeed, averageSpeed), GetScreenWidth()*(0.3), GetScreenHeight()*(0.0125), 18, BLACK);

        EndDrawing();
        frameCounter++;
    }

    CloseWindow();

    HistogramRelease(&histogram);
    UnloadRenderTexture(particleRenderTex);
    UnloadRenderTexture(boxTexture);
    UnloadTexture(nPatchTexture);
    return EXIT_SUCCESS;
}


 

void HistogramCompute(const Particle* particles, size_t n_part, Histogram* histogram)
{
    float Binwidth = (histogram->max - histogram->min) / histogram->Nbins;
    size_t bin_number;
    for (size_t i = 0; i < N_part; i++)
    {
        bin_number = (size_t)(  ParticleGetVelocity(&particles[i]) / Binwidth  );
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

