#include "particle_lib/particle.h"
#include "particle_lib/monte_carlo_particle_position_setter.h"
#include "histogram.h"
#include <time.h>

#define WINDOW_WIDTH             1528UL
#define WINDOW_HEIGHT            960UL
#define N_PARTICLES              2000UL
#define NO_ROTATION              0.f
#define NO_ZOOM                  1.f
#define THICKNESS                2.f
#define BACKGROUND_COLOR         RAYWHITE
#define PARTICLE_RADIUS          4.f
#define PARTICLE_VELOCITY        25.f//pixel/sec
#define PARTICLE_MASS            1.f    
#define UPDATE_FREQUENCY/*every*/3/*frame*/ 
#define HISTO_NBINS              20UL
#define HISTO_MIN_SPEED          0.f
#define HISTO_MAX_SPEED          50.f

int main(void)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Maxwell-Boltzmann distribution");
    unsigned int seed = (unsigned int)time(NULL);
    srand(seed);
    SetRandomSeed(seed); 
    // <-- Box for particles --> 
    Box box = 
    {
        .xLeft   =   (int)(0.05*WINDOW_WIDTH),       
        .xRight  =   (int)(0.55*WINDOW_WIDTH),
        .yTop    =   (int)(0.05*WINDOW_HEIGHT),
        .yBottom =   (int)(0.95*WINDOW_HEIGHT)
    };

    // < -- Particles --> 
    Vector2 pos = {0};
    Vector2 vel = {0};
    
    Particle particles[N_PARTICLES];
    for (size_t i = 0; i < N_PARTICLES; i++)
    {
        vel = (Vector2){GetRandomValue(-PARTICLE_VELOCITY, PARTICLE_VELOCITY), GetRandomValue(-PARTICLE_VELOCITY, PARTICLE_VELOCITY)};
        ParticleSet(&particles[i], pos, vel, PARTICLE_RADIUS, PARTICLE_MASS);
    }
    ParticleRemoveCenterOfMass(particles, N_PARTICLES);


    puts("[INFO]: MonteCarlo set positions: Start");
    for (size_t n = 0; n < N_PARTICLES; n++)
    {
        size_t particleNumberToCheck = n;
        if(!MonteCarloUpdatePosition(n, particles, particleNumberToCheck, &box, PARTICLE_RADIUS))
        {
            fprintf(stderr, "Monte Carlo position update does not converge after %u trials\n", MONTECARLO_MAX_ATTEMPT);
            CloseWindow();
            return EXIT_FAILURE;
        }
    }
    puts("[INFO]: MonteCarlo set positions: finished");


    Histogram histogram = {0};
    if(!HistogramInit(&histogram, HISTO_NBINS, HISTO_MIN_SPEED, HISTO_MAX_SPEED))
    {
        fprintf(stderr, "Could not init Histogram: allocation fail");
        CloseWindow();
        return EXIT_FAILURE;
    }

    RenderTexture2D particleRenderTex = ParticleCreateRenderTexture(particles[1].radius, BLACK);

    //Histogram Rectangle;
    float xStart            = 0.6f*WINDOW_WIDTH;
    float yStart            = 0.9f*WINDOW_HEIGHT;
    float binRecWidth       = roundf(0.8f*(WINDOW_WIDTH-xStart)/HISTO_NBINS); 
    float binRecHeightIinit = 32.0f;

    float maxHeight         = 0.8f*WINDOW_HEIGHT;
    float scalingFactor     = maxHeight/(0.15f*N_PARTICLES); // scaling between binValues --> binRectangle height on the screen


    Texture2D nPatchTexture = LoadTexture("ninepatch_button.png");
    NPatchInfo v3PatchInfo  = {(Rectangle){0.0f, 192.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH}; // from raylib
    Vector2 origin          = {0};  

    Rectangle bins[HISTO_NBINS];
    for (size_t n = 0; n < HISTO_NBINS; n++)
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
        ParticleCheckCollisions(particles, N_PARTICLES);
        for (size_t i = 0; i < N_PARTICLES; i++)
        {
            ParticleCheckBoundary(&particles[i], &box); //Implicit Euler: First update velocity, then update positions accordingly.
            ParticleMove(&particles[i], dt);
        }

        if(frameCounter%UPDATE_FREQUENCY == 0)
        {
            ParticleUpdateInfo(&minSpeed, &maxSpeed, &averageSpeed, particles, N_PARTICLES);
            HistogramCompute(particles, N_PARTICLES, &histogram);
            for (size_t n = 0; n < HISTO_NBINS; n++)
            {
                HistogramFillBins(&bins[n], histogram.binValues[n], scalingFactor, maxHeight);
            }
            memset(histogram.binValues, 0, HISTO_NBINS*sizeof(uint64_t));
            if(frameCounter == UINT64_MAX)
            {
                frameCounter = 0;
            }
        }

        BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);
            for (size_t i = 0; i < N_PARTICLES; i++)
            {
                DrawTextureV(particleRenderTex.texture, ParticleGetCenter(&particles[i]), WHITE);
            }
            for (size_t n = 0; n < HISTO_NBINS; n++)
            {
                DrawTextureNPatch(nPatchTexture, v3PatchInfo, bins[n], origin, NO_ROTATION, WHITE);
            }
            BoxRender(&box, THICKNESS, BLACK);
            DrawFPS(GetScreenWidth()*(0.05), GetScreenHeight()*(0.0125));
            DrawText(TextFormat("SpeedInfo: Min:%.2f, Max:%.2f, avg:%.2f", minSpeed, maxSpeed, averageSpeed), GetScreenWidth()*(0.3), GetScreenHeight()*(0.0125), 18, BLACK);

        EndDrawing();
        frameCounter++;
    }

    CloseWindow();
    HistogramRelease(&histogram);
    UnloadRenderTexture(particleRenderTex);
    UnloadTexture(nPatchTexture);
    return EXIT_SUCCESS;
}
 