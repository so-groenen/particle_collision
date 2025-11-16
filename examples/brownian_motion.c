// #include "vectorAlgebra.h"
#include "particle.h"
#include "monte_carlo_particle_position_setter.h"
#include <time.h>

#define N_part 3000UL


// void ParticleUpdateInfo(float* MinSpeed, float* MaxSpeed, float* AverageSpeed, const Particle* particles, size_t N);
 

int main(void)
{
    const uint64_t windowWidth  = 480;
    const uint64_t windoWheight = 480;
    InitWindow(windowWidth, windoWheight, "raylib [core] example - basic window");
    
    SetRandomSeed(123);
    srand(time(NULL));

    // <-- Box for particles --> 
    Box Box = 
    {
        .xLeft   =   windowWidth*(0.05),       
        .xRight  =   (0.95)*windowWidth,
        .yTop    =   windoWheight*(0.05),
        .yBottom =   (0.95)*windoWheight
    };
    RenderTexture2D boxTexture = LoadRenderTexture(windowWidth, windoWheight);
    
    const float thickness = 1.0f;
    BeginTextureMode(boxTexture);
        ClearBackground(RAYWHITE);
        DrawLineEx((Vector2){Box.xLeft,  Box.yBottom}, (Vector2){Box.xLeft,  Box.yTop},    thickness, BLACK);
        DrawLineEx((Vector2){Box.xRight, Box.yBottom}, (Vector2){Box.xRight, Box.yTop},    thickness, BLACK);
        DrawLineEx((Vector2){Box.xLeft,  Box.yBottom}, (Vector2){Box.xRight, Box.yBottom}, thickness, BLACK);
        DrawLineEx((Vector2){Box.xLeft,  Box.yTop},    (Vector2){Box.xRight, Box.yTop},    thickness, BLACK);
    EndTextureMode();


    // < -- Particles --> 
    // const size_t N_part  = 3000;
    const float radius   = 2;
    const float velocity = 60; //  pixel / secondes
    const float mass     = 1;
    Vector2 pos = {0};
    Vector2 vel = {0};
    
    const float BrownianRadius = 40;
    const float BrownianMass   = 5;
    size_t BrownianIndex       = 0;

    Particle particle[N_part];

    for (size_t i = 0; i < N_part; i++)
    {
        vel = (Vector2){GetRandomValue(-velocity, velocity), GetRandomValue(-velocity, velocity)};
        ParticleSet(&particle[i], vel, radius, mass);
    }
    // ParticleRemoveCenterOfMass(particle, N_part);


    particle[BrownianIndex].mass   = BrownianMass;
    particle[BrownianIndex].radius = BrownianRadius;

    MonteCarloUpdatePosition(BrownianIndex, particle, 0, &Box, radius);
    for (size_t n = 1; n < N_part; n++)
    {
        size_t ParticleNumberToCheck = n;
        MonteCarloUpdatePosition(n, particle, ParticleNumberToCheck, &Box, radius);
    }

    RenderTexture2D particleRenderTex = ParticleCreateRenderTexture(particle[1].radius);

    // RenderTexture2D BronwianRenderer = ParticleCreateRenderTexture(particle[BrownianIndex].radius);
    Texture2D BrownianWeary = LoadTexture("weary-face-emoji-clipart-md.png");
    // <-- histogram -->
    int frameWidth  = BrownianWeary.width;
    int frameHeight = BrownianWeary.height;

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    Rectangle destRec   = { particle[BrownianIndex].pos.x, particle[BrownianIndex].pos.y, 2*BrownianRadius, 2*BrownianRadius};
    Vector2 origin      = {0,0};

 
    // <-- Simulation Start -->
    uint64_t targetFPS    = 100;
    uint64_t frameCounter = 0;

    float dt =  1.0f / (float)targetFPS;

    float MinSpeed, MaxSpeed, AverageSpeed;

    // SetTargetFPS(targetFPS);
    while (!WindowShouldClose())
    {
        
        // dt = GetFrameTime();
    
        ParticleCheckCollisions(particle, N_part);
        for (size_t i = 0; i < N_part; i++)
        {
            ParticleCheckBoundary(&particle[i], &Box); //Implicit Euler: First update velocity, then update positions accordingly.
            ParticleMove(&particle[i], dt);
        }
        if(frameCounter%10 == 0 )
        {
            ParticleUpdateInfo(&MinSpeed, &MaxSpeed, &AverageSpeed, particle, N_part);
        }


        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTextureV(boxTexture.texture, (Vector2){0, 0}, WHITE); // when drawing the particles, the position is the edge of the texture, not the center. We can offset it here, but also at the particle level
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
                    DrawTextureV(particleRenderTex.texture, (Vector2){particle[i].pos.x - particle[i].radius, particle[i].pos.y - particle[i].radius}, WHITE);
                }   
            }
            DrawFPS(GetScreenWidth()*(0.05), GetScreenHeight()*(0.0125));
            DrawText(TextFormat("SpeedInfo: Min:%.2f, Max:%.2f, avg:%.2f", MinSpeed, MaxSpeed, AverageSpeed), GetScreenWidth()*(0.3), GetScreenHeight()*(0.0125), 18, BLACK);

        EndDrawing();
        frameCounter++;
    }

    CloseWindow();
    UnloadRenderTexture(particleRenderTex);
    UnloadTexture(BrownianWeary);
    UnloadRenderTexture(boxTexture);
    return 0;
}



 