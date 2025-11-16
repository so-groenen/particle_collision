#include "particle_lib/particle.h"
#include "particle_lib/monte_carlo_particle_position_setter.h"
#include <time.h>

#define WINDOW_WIDTH     1028UL
#define WINDOW_HEIGHT    960UL

#define N_PARTICLES      3000UL
#define BROWNIAN_INDEX   0UL
#define NO_ROTATION      0.f

#define THICKNESS        2.f
#define BACKGROUND_COLOR RAYWHITE


enum DrawMode
{
    FullParticles = 0,
    BrownianPath  = 1,
};
 

int main(void)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Brownian Motion");
    
    SetRandomSeed(123);
    srand(time(NULL));

    // <-- Box for particles --> 
    Box box = 
    {
        .xLeft   =   WINDOW_WIDTH*(0.05),       
        .xRight  =   (0.95)*WINDOW_WIDTH,
        .yTop    =   WINDOW_HEIGHT*(0.05),
        .yBottom =   (0.95)*WINDOW_HEIGHT
    };

    // < -- Particles --> 
    const float radius   = 4.f;
    const float velocity = 80.f; //  pixel / secondes
    const float mass     = 1.f;
    Vector2 pos          = {0};
    Vector2 vel          = {0};
    
    const float brownianRadius = 40.f;
    const float brownianMass   = 4.f;

    // Setup particles
    Particle particles[N_PARTICLES];
    for (size_t i = 0; i < N_PARTICLES; i++)
    {
        vel = (Vector2){GetRandomValue(-velocity, velocity), GetRandomValue(-velocity, velocity)};
        ParticleSet(&particles[i], pos, vel, radius, mass);
    }
    RenderTexture2D particleRenderTex = ParticleCreateRenderTexture(particles[1].radius, BLACK);

    particles[BROWNIAN_INDEX].mass   = brownianMass;
    particles[BROWNIAN_INDEX].radius = brownianRadius;

    MonteCarloUpdatePosition(BROWNIAN_INDEX, particles, 0, &box, radius);
    for (size_t n = 1; n < N_PARTICLES; n++)
    {
        size_t ParticleNumberToCheck = n;
        MonteCarloUpdatePosition(n, particles, ParticleNumberToCheck, &box, radius);
    }

    // Brownian particle texture:
    Texture2D brownianTexture         = LoadTexture("sphere.png");
    int frameWidth                    = brownianTexture.width;
    int frameHeight                   = brownianTexture.height;
    Rectangle sourceRec               = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    Rectangle destRec   = { particles[BROWNIAN_INDEX].pos.x, particles[BROWNIAN_INDEX].pos.y, 2*brownianRadius, 2*brownianRadius};
    Vector2 origin      = {0};

    // Setup canvas to draw the path
    RenderTexture2D canvas  = LoadRenderTexture(BoxGetWidth(&box), BoxGetHeigth(&box));
    BeginTextureMode(canvas);
        ClearBackground(BACKGROUND_COLOR);
    EndTextureMode();

    enum DrawMode drawMode = FullParticles;

    // <-- Simulation Start -->
    uint64_t targetFPS    = 100;
    uint64_t frameCounter = 0;

    float dt        =  1.0f / (float)targetFPS;
    float minSpeed, maxSpeed, averageSpeed;
    Vector2 upperLeft = BoxGetUpperLeft(&box);
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
        if(frameCounter%10 == 0 )
        {
            ParticleUpdateInfo(&minSpeed, &maxSpeed, &averageSpeed, particles, N_PARTICLES);
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            drawMode ^= 1;
            if (drawMode == BrownianPath)
            {
                BeginTextureMode(canvas);
                    ClearBackground(BACKGROUND_COLOR);
                EndTextureMode();     
            }   
        }

        if (drawMode == BrownianPath)
        {
            BeginTextureMode(canvas);
                DrawCircleV(VectorSubtract(&particles[BROWNIAN_INDEX].pos, &upperLeft), 2, BLUE);
            EndTextureMode();
        }


        BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);
            if (drawMode == BrownianPath)
            {
                DrawTextureRec(canvas.texture, (Rectangle) {0, 0, (float)canvas.texture.width, (float)-canvas.texture.height }, upperLeft, WHITE);
            }
            else
            {
                Vector2 center = ParticleGetCenter(&particles[BROWNIAN_INDEX]);
                destRec.x      = center.x;
                destRec.y      = center.y;
                DrawTexturePro(brownianTexture, sourceRec, destRec, origin, NO_ROTATION, WHITE);
                for (size_t i = BROWNIAN_INDEX+1; i < N_PARTICLES; i++)
                {
                    DrawTextureV(particleRenderTex.texture, ParticleGetCenter(&particles[i]), WHITE);
                }
            }
            BoxRender(&box, THICKNESS, BLACK);
            DrawFPS(WINDOW_WIDTH*(0.05), WINDOW_HEIGHT*(0.0125));
            DrawText(TextFormat("SpeedInfo: Min:%.2f, Max:%.2f, avg:%.2f", minSpeed, maxSpeed, averageSpeed), WINDOW_WIDTH*(0.3), WINDOW_HEIGHT*(0.0125), 18, BLACK);
        EndDrawing();
        frameCounter++;
    }

    CloseWindow();
    UnloadRenderTexture(particleRenderTex);
    // UnloadRenderTexture(emptyParticleRenderTex);
    UnloadTexture(brownianTexture);
    // UnloadRenderTexture(boxTexture);
    UnloadRenderTexture(canvas);
    return EXIT_SUCCESS;
}



 