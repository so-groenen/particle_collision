#include "particle.h"
#include "monte_carlo_particle_position_setter.h"
#include <time.h>

#define N_part 3000UL



int main(void)
{
    const uint64_t windowWidth  = 480;
    const uint64_t windoWheight = 480;
    InitWindow(windowWidth, windoWheight, "raylib [core] example - basic window");
    
    SetRandomSeed(123);
    srand(time(NULL));

    // <-- Box for particles --> 
    Box box = 
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
        DrawLineEx((Vector2){box.xLeft,  box.yBottom}, (Vector2){box.xLeft,  box.yTop},    thickness, BLACK);
        DrawLineEx((Vector2){box.xRight, box.yBottom}, (Vector2){box.xRight, box.yTop},    thickness, BLACK);
        DrawLineEx((Vector2){box.xLeft,  box.yBottom}, (Vector2){box.xRight, box.yBottom}, thickness, BLACK);
        DrawLineEx((Vector2){box.xLeft,  box.yTop},    (Vector2){box.xRight, box.yTop},    thickness, BLACK);
    EndTextureMode();


    // < -- Particles --> 
    const float radius   = 2.f;
    const float velocity = 60.f; //  pixel / secondes
    const float mass     = 1.f;
    Vector2 pos          = {0};
    Vector2 vel          = {0};
    
    const float brownianRadius = 40.f;
    const float BrownianMass   = 5.f;
    size_t brownianIndex       = 0.f;

    Particle particle[N_part];

    for (size_t i = 0; i < N_part; i++)
    {
        vel = (Vector2){GetRandomValue(-velocity, velocity), GetRandomValue(-velocity, velocity)};
        ParticleSet(&particle[i], vel, radius, mass);
    }
    // ParticleRemoveCenterOfMass(particle, N_part);


    particle[brownianIndex].mass   = BrownianMass;
    particle[brownianIndex].radius = brownianRadius;

    MonteCarloUpdatePosition(brownianIndex, particle, 0, &box, radius);
    for (size_t n = 1; n < N_part; n++)
    {
        size_t ParticleNumberToCheck = n;
        MonteCarloUpdatePosition(n, particle, ParticleNumberToCheck, &box, radius);
    }

    RenderTexture2D particleRenderTex = ParticleCreateRenderTexture(particle[1].radius);
    Texture2D brownianTexture         = LoadTexture("weary-face-emoji-clipart-md.png");

    // RenderTexture2D BronwianRenderer = ParticleCreateRenderTexture(particle[brownianIndex].radius);
    // <-- histogram -->
    int frameWidth  = brownianTexture.width;
    int frameHeight = brownianTexture.height;

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    Rectangle destRec   = { particle[brownianIndex].pos.x, particle[brownianIndex].pos.y, 2*brownianRadius, 2*brownianRadius};
    Vector2 origin      = {0,0};

 
    // <-- Simulation Start -->
    uint64_t targetFPS    = 100;
    uint64_t frameCounter = 0;

    float dt =  1.0f / (float)targetFPS;

    float minSpeed, maxSpeed, averageSpeed;

    // SetTargetFPS(targetFPS);
    while (!WindowShouldClose())
    {
        
        // dt = GetFrameTime();
    
        ParticleCheckCollisions(particle, N_part);
        for (size_t i = 0; i < N_part; i++)
        {
            ParticleCheckBoundary(&particle[i], &box); //Implicit Euler: First update velocity, then update positions accordingly.
            ParticleMove(&particle[i], dt);
        }
        if(frameCounter%10 == 0 )
        {
            ParticleUpdateInfo(&minSpeed, &maxSpeed, &averageSpeed, particle, N_part);
        }


        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTextureV(boxTexture.texture, (Vector2){0, 0}, WHITE); // when drawing the particles, the position is the edge of the texture, not the center. We can offset it here, but also at the particle level
            for (size_t i = 0; i < N_part; i++)
            {
                if (i == brownianIndex)
                {
                    destRec.x = particle[brownianIndex].pos.x - brownianRadius;
                    destRec.y = particle[brownianIndex].pos.y - brownianRadius;
                    DrawTexturePro(brownianTexture, sourceRec, destRec, origin, 0.0f, WHITE);
                }
                else
                {
                    DrawTextureV(particleRenderTex.texture, (Vector2){particle[i].pos.x - particle[i].radius, particle[i].pos.y - particle[i].radius}, WHITE);
                }   
            }
            DrawFPS(GetScreenWidth()*(0.05), GetScreenHeight()*(0.0125));
            DrawText(TextFormat("SpeedInfo: Min:%.2f, Max:%.2f, avg:%.2f", minSpeed, maxSpeed, averageSpeed), GetScreenWidth()*(0.3), GetScreenHeight()*(0.0125), 18, BLACK);

        EndDrawing();
        frameCounter++;
    }

    CloseWindow();
    UnloadRenderTexture(particleRenderTex);
    UnloadTexture(brownianTexture);
    UnloadRenderTexture(boxTexture);
    return EXIT_SUCCESS;
}



 