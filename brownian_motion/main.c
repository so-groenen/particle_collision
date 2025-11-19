#include "particle_lib/particle.h"
#include "particle_lib/monte_carlo_particle_position_setter.h"
#include <time.h>

#define WINDOW_WIDTH        1028UL
#define WINDOW_HEIGHT       960UL
#define N_PARTICLES         2000UL
#define BROWNIAN_INDEX      0UL
#define NO_ROTATION         0.f
#define NO_ZOOM             1.f
#define THICKNESS           2.f
#define BACKGROUND_COLOR    RAYWHITE
#define TRACE_CIRCLE_RADIUS 2
#define PARTICLE_RADIUS     4.f
#define PARTICLE_VELOCITY   80.f
#define PARTICLE_MASS       1.f    
#define BROWNIAN_RADIUS     40.f
#define BROWNIAN_MASS       4.f

enum DrawMode
{
    FullParticles = 0,
    BrownianPath  = 1,
};
 

int main(void)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Brownian Motion");
    SetRandomSeed((unsigned int)time(NULL));
 
    // <-- Box for particles --> 
    const Box box = 
    {
        .xLeft   = 0,       
        .xRight  = (int)(WINDOW_WIDTH*0.9),
        .yTop    = 0,
        .yBottom = (int)(WINDOW_HEIGHT*0.9)
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
    RenderTexture2D particleRenderTex = ParticleCreateRenderTexture(particles[1].radius, BLACK);

    particles[BROWNIAN_INDEX].mass   = BROWNIAN_MASS;
    particles[BROWNIAN_INDEX].radius = BROWNIAN_RADIUS;


    puts("[INFO]: MonteCarlo set positions: Start");
    MonteCarloUpdatePosition(BROWNIAN_INDEX, particles, BROWNIAN_INDEX, &box, PARTICLE_RADIUS);
    for (size_t n = BROWNIAN_INDEX+1; n < N_PARTICLES; n++)
    {
        size_t particleNumberToCheck = n;
        if(!MonteCarloUpdatePosition(n, particles, particleNumberToCheck, &box, PARTICLE_RADIUS))
        {
            fprintf(stderr, "Monte Carlo position update does not converge after %u trials\n", MONTECARLO_MAX_ATTEMPT);
            CloseWindow();
            UnloadRenderTexture(particleRenderTex);
            return EXIT_FAILURE;
        }
    }
    puts("[INFO]: MonteCarlo set positions: Finish");


    // Brownian particle texture:
    Texture2D brownianTexture = LoadTexture("sphere.png");
    unsigned int frameWidth   = brownianTexture.width;
    unsigned int frameHeight  = brownianTexture.height;
    Vector2 origin            = {0};
    Rectangle sourceRec       = {0.f, 0.f, (float)frameWidth, (float)frameHeight };
    Rectangle destRec         = {particles[BROWNIAN_INDEX].pos.x, particles[BROWNIAN_INDEX].pos.y, 2*BROWNIAN_RADIUS, 2*BROWNIAN_RADIUS};

    // Setup canvas to draw the path
    RenderTexture2D canvas  = LoadRenderTexture(BoxGetWidth(&box), BoxGetHeigth(&box));
    BeginTextureMode(canvas);
        ClearBackground(BACKGROUND_COLOR);
    EndTextureMode();

    // center on box
    Camera2D camera = 
    {
        .zoom     = NO_ZOOM,
        .rotation = NO_ROTATION,
        .offset   = (Vector2){WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f},
        .target   = BoxGetCenter(&box),
    };

    enum DrawMode drawMode = FullParticles;

    // <-- display infos -->
    float minSpeed     = 0.f;
    float maxSpeed     = 0.f;
    float averageSpeed = 0.f;


    uint64_t targetFPS    = 100;
    uint64_t frameCounter = 0;
    float dt              = 1.0f / (float)targetFPS;
    while (!WindowShouldClose())
    {
        ParticleCheckCollisions(particles, N_PARTICLES);
        for (size_t i = 0; i < N_PARTICLES; i++)
        {
            ParticleCheckBoundary(&particles[i], &box); //Implicit Euler: First update velocity, then update positions accordingly.
            ParticleMove(&particles[i], dt);
        }
        if(frameCounter%10 == 0)
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
                DrawCircleV(particles[BROWNIAN_INDEX].pos, TRACE_CIRCLE_RADIUS, BLUE);
            EndTextureMode();
        }

        BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);
            BeginMode2D(camera);
                if (drawMode == BrownianPath)
                {
                    DrawTextureRec(canvas.texture, (Rectangle) {0, 0, (float)canvas.texture.width, (float)-canvas.texture.height }, origin, WHITE);
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
            EndMode2D();

            DrawFPS(WINDOW_WIDTH*(0.05f), WINDOW_HEIGHT*(0.0125f));
            DrawText(TextFormat("SpeedInfo: Min:%.2f, Max:%.2f, avg:%.2f", minSpeed, maxSpeed, averageSpeed), WINDOW_WIDTH*(0.3f), WINDOW_HEIGHT*(0.0125f), 18, BLACK);
        EndDrawing();
        frameCounter++;
    }

    CloseWindow();
    UnloadRenderTexture(particleRenderTex);
    UnloadTexture(brownianTexture);
    UnloadRenderTexture(canvas);
    return EXIT_SUCCESS;
}



 