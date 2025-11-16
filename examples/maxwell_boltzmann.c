#include "vectorAlgebra.h"
#include "particle.h"
#include "MonteCarloOverlapRemover.h"
#include <time.h>

typedef struct
{
    float min;
    float max;
    uint64_t* binValues;
    size_t    Nbins;
} histogram_t;


void updateParticleInfo(float* MinSpeed, float* MaxSpeed, float* AverageSpeed, const particle_t* particles, size_t N);
void computeHistogram(const particle_t* particles, size_t N_part, histogram_t* histogram);
void initHistogram(histogram_t* histogram, size_t Nbins, float min, float max);
void freeHistogram(histogram_t* histogram);
void FillHistogramBins(Rectangle* bin, uint64_t BinValue, float scalingFactor, float MaxHeight);


int main(void)
{
    const uint64_t WindowWidth  = 480;
    const uint64_t WindoWheight = 480;
    InitWindow(WindowWidth, WindoWheight, "raylib [core] example - basic window");
    
    SetRandomSeed(123);
    srand(time(NULL));

    // <-- Box for particles --> 
    Box_t Box = {.xLeft   =   WindowWidth*(0.05),       
                 .xRight  =   (0.95)*WindowWidth,
                 .yTop    =   WindoWheight*(0.05),
                 .yBottom =   (0.95)*WindoWheight
                };

    RenderTexture2D BoxText = LoadRenderTexture(WindowWidth, WindoWheight);
    
    const float thickness = 1;
    BeginTextureMode(BoxText);
        ClearBackground(RAYWHITE);
        DrawLineEx((Vector2){Box.xLeft,  Box.yBottom}, (Vector2){Box.xLeft,  Box.yTop},    thickness, BLACK);
        DrawLineEx((Vector2){Box.xRight, Box.yBottom}, (Vector2){Box.xRight, Box.yTop},    thickness, BLACK);
        DrawLineEx((Vector2){Box.xLeft,  Box.yBottom}, (Vector2){Box.xRight, Box.yBottom}, thickness, BLACK);
        DrawLineEx((Vector2){Box.xLeft,  Box.yTop},    (Vector2){Box.xRight, Box.yTop},    thickness, BLACK);
    EndTextureMode();


    // < -- Particles --> 
    const size_t N_part  = 3000;
    const float radius   = 2;
    const float velocity = 60; //  pixel / secondes
    const float mass     = 1;
    Vector2 pos;
    Vector2 vel;
    
    const float BrownianRadius = 40;
    const float BrownianMass   = 5;
    size_t BrownianIndex       = 0;

    particle_t particle[N_part];

    for (size_t i = 0; i < N_part; i++)
    {
        vel = (Vector2){GetRandomValue(-velocity, velocity), GetRandomValue(-velocity, velocity)};
        initParticle(&particle[i], vel, radius, mass);
    }
    // ParticleRemoveCenterOfMass(particle, N_part);


    particle[BrownianIndex].mass   = BrownianMass;
    particle[BrownianIndex].radius = BrownianRadius;

    MonteCarlo_UpdateParticlePosition(BrownianIndex, particle, 0, &Box, radius);
    for (size_t n = 1; n < N_part; n++)
    {
        size_t ParticleNumberToCheck = n;
        MonteCarlo_UpdateParticlePosition(n, particle, ParticleNumberToCheck, &Box, radius);
    }

    RenderTexture2D ParticleRenderer = createParticleRenderer(particle[1].radius);

    // RenderTexture2D BronwianRenderer = createParticleRenderer(particle[BrownianIndex].radius);
    Texture2D BrownianWeary = LoadTexture("weary-face-emoji-clipart-md.png");
    // <-- histogram -->
    int frameWidth  = BrownianWeary.width;
    int frameHeight = BrownianWeary.height;

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    Rectangle destRec   = { particle[BrownianIndex].pos.x, particle[BrownianIndex].pos.y, 2*BrownianRadius, 2*BrownianRadius};
    Vector2 origin = {0,0};


    // histogram_t histogram;
    // const float min         = 0;
    // const float max         = 45;
    // const size_t Nbins      = 20;
    // initHistogram(&histogram, Nbins, min, max);

    //     //Histogram Rectangle;
    // float xStart           = (0.6)*WindowWidth;
    // float yStart           = (0.9)*WindoWheight;
    // float binRecWidth      = 16.0f;
    // float binRecheightINIT = 32.0f;

    // float scalingFactor    = 3; // scaling between BinValues --> binRectangle height on the screen.
    // float MaxHeight        = (0.8)*GetScreenHeight();


    // Texture2D nPatchTexture = LoadTexture("ninepatch_button.png");
    // NPatchInfo v3PatchInfo  = { (Rectangle){ 0.0f, 192.0f, 64.0f, 64.0f }, 6, 6, 6, 6, NPATCH_NINE_PATCH  }; // position of texture in the png
    // Vector2 origin          = {0.0f, 0.0f}; // for rotation, but rotation is kept at 0.

    // Rectangle bins[Nbins];
    // for (size_t n = 0; n < Nbins; n++)
    // {
    //     bins[n] = (Rectangle){xStart + n*binRecWidth, yStart, binRecWidth, binRecheightINIT};
    // }
    
    // <-- Simulation Start -->
    uint64_t targetFPS    = 100;
    uint64_t frameCounter = 0;

    float dt =  1.0f / (float)targetFPS;

    float MinSpeed, MaxSpeed, AverageSpeed;

    // SetTargetFPS(targetFPS);
    while (!WindowShouldClose())
    {
        
        // dt = GetFrameTime();
    
        // <-- particle updates --> //
        ParticleCheckCollisions(particle, N_part);
        for (size_t i = 0; i < N_part; i++)
        {
            ParticleCheckBoundary(&particle[i], &Box); //Implicit Euler: First update velocity, then update positions accordingly.
            ParticleMove(&particle[i], dt);
        }

        // <-- Histogram update -->
        if(frameCounter%10 == 0 )
        {
            updateParticleInfo(&MinSpeed, &MaxSpeed, &AverageSpeed, particle, N_part);
            // computeHistogram(particle, N_part, &histogram);
            // for (size_t n = 0; n < Nbins; n++)
            // {
            //     FillHistogramBins(&bins[n], histogram.binValues[n], scalingFactor, MaxHeight);
            // }
            // memset(histogram.binValues, 0, Nbins*sizeof(uint64_t));

            // if(frameCounter == UINT64_MAX - 1)
            // {
            //     frameCounter = 0;
            // }
        }


        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTextureV(BoxText.texture, (Vector2){0, 0}, WHITE); // when drawing the particles, the position is the edge of the texture, not the center. We can offset it here, but also at the particle level
            for (size_t i = 0; i < N_part; i++)
            {
                if (i == BrownianIndex)
                {
                    destRec.x = particle[BrownianIndex].pos.x - BrownianRadius;
                    destRec.y = particle[BrownianIndex].pos.y - BrownianRadius;
                    DrawTexturePro(BrownianWeary, sourceRec, destRec, origin, 0.0f, WHITE);
                    // DrawTextureV(BronwianRenderer.texture, (Vector2){particle[i].pos.x - particle[i].radius, particle[i].pos.y - particle[i].radius}, WHITE);
                }
                else
                {
                    DrawTextureV(ParticleRenderer.texture, (Vector2){particle[i].pos.x - particle[i].radius, particle[i].pos.y - particle[i].radius}, WHITE);
                }   
            }
            // for (size_t n = 0; n < Nbins; n++)
            // {
            //     DrawTextureNPatch(nPatchTexture, v3PatchInfo, bins[n], origin, 0.0f, WHITE);
            // }
            DrawFPS(GetScreenWidth()*(0.05), GetScreenHeight()*(0.0125));
            DrawText(TextFormat("SpeedInfo: Min:%.2f, Max:%.2f, avg:%.2f", MinSpeed, MaxSpeed, AverageSpeed), GetScreenWidth()*(0.3), GetScreenHeight()*(0.0125), 18, BLACK);

        EndDrawing();
        frameCounter++;
    }

    CloseWindow();

    // freeHistogram(&histogram);
    UnloadRenderTexture(ParticleRenderer);
    // UnloadRenderTexture(BronwianRenderer);
    UnloadTexture(BrownianWeary);
    UnloadRenderTexture(BoxText);
    // UnloadTexture(nPatchTexture);
    return 0;
}




void updateParticleInfo(float* MinSpeed, float* MaxSpeed, float* AverageSpeed, const particle_t* particles, size_t N)
{
    float sum = 0;
    float min = 1E6;
    float max = 0;
    float speed = 0;
    for (size_t i = 0; i < N; i++)
    {
        speed = getSpeed(&particles[i]);
        if (speed < min)
        {
            min = speed;
        }
        if (speed > max)
        {
            max = speed;
        }
        sum += speed;
    }
    (*MinSpeed)     = min;
    (*MaxSpeed)     = max;
    (*AverageSpeed) = (sum/(float)N);
}


void computeHistogram(const particle_t* particles, size_t N_part, histogram_t* histogram)
{
    float Binwidth = (histogram->max - histogram->min) / histogram->Nbins;
    size_t bin_number;
    for (size_t i = 0; i < N_part; i++)
    {
        bin_number = (size_t)(  getSpeed(&particles[i]) / Binwidth  );
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


void initHistogram(histogram_t* histogram, size_t Nbins, float min, float max)
{
    uint64_t* data = calloc(Nbins, sizeof(uint64_t));
    histogram->binValues = data;
    histogram->max = max;
    histogram->max = max;
    histogram->Nbins = Nbins;    
}

void freeHistogram(histogram_t* histogram)
{
    free(histogram->binValues);
}



void FillHistogramBins(Rectangle* bin, uint64_t BinValue, float scalingFactor, float MaxHeight)
{
    float oldHeight = bin->height;
    float NewHeight = (scalingFactor * BinValue);

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

