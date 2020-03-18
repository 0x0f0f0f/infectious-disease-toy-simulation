#include <stdlib.h>
#include <raylib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define RAND_RANGE(min, max) \
    (rand() % (max + 1 - min)) + min

#define RAND_FLOAT (float)rand()/RAND_MAX

#define INFECTED PURPLE

#define A b[i]
#define B b[j]

typedef struct __ball {
    Vector2 pos;
    Vector2 vel;
    Color c;
    long long int infected;
    long long int immune;
    int alive;
} ball;

int main(void) {
    /* Max simulation time in seconds */
    const int maxtime = 40;
    const int fps = 60;
    const int screen_w = 1280;
    const int screen_h = 720;
    /* Probability of infecting another individual */
    const double infectivity = 0.57;
    /* Probability of death */
    const double mortality = 0.1;
    /* Healing time in frames */
    const double healing_time = 4 * fps;
    /* 1/n people that are not staying home */
    const int people_count = 800;
    const int not_staying_home = 4;
    const int graph_height = 128;
    const int br = 2;
    /* How many frames per sample */
    const int sampling_rate = 5;
    const int graph_width = (maxtime * fps) / sampling_rate;
    const int graph_max_width = screen_w - (graph_height * 3 );


    int i = 0, j = 0, paused = 0, bar_infected_height, bar_immune_height, bar_dead_height, bar_width;
    long long int fc = 0;
    long int graph_instant;
    ball *b = malloc(sizeof(ball) * people_count);
    char *msg = malloc(2048);

    int *infected_count = calloc(graph_width, sizeof(int));
    int *immune_count = calloc(graph_width, sizeof(int));
    int *dead_count = calloc(graph_width, sizeof(int));


    /* Init random number generator */
    srand(time(NULL));

    /* Init the window */
    InitWindow(screen_w, screen_h, "demo");
    SetTargetFPS(fps);

    /* Init the scene */
    fc = 0;
    graph_instant = 0;
    for (i = 0; i < people_count; i++) {
        A.pos = (Vector2) {
            RAND_RANGE(br, GetScreenWidth() - br),
            RAND_RANGE(br, GetScreenHeight() - br - graph_height)
        };
        if (i % not_staying_home == 0)
            A.vel = (Vector2){ RAND_RANGE(1,3), RAND_RANGE(1, 3) };
        else A.vel = (Vector2) {0, 0};
        A.c = GRAY;
        A.immune = 0;
        A.infected = 0;
        A.alive = true;
    }

    /* A single random ball is infected */
    int first_infected = RAND_RANGE(0, people_count - 1);
    first_infected = first_infected - (first_infected % not_staying_home);
    b[first_infected].infected = 1;
    b[first_infected].c = INFECTED;

    /* SIMULATION LOOP */
    while(!WindowShouldClose()) {
        if(fc >= maxtime*fps) paused = true;

        // Custom pause
        if (IsKeyPressed(' ') && fc <= maxtime * fps) {
            paused = !paused;
        }

        if(!paused) {
            fc++;
            graph_instant = fc/sampling_rate;
            dead_count[graph_instant] = 0;
            immune_count[graph_instant] = 0;
            infected_count[graph_instant] = 0;
        }

        /* Updating Loop */
        for (i = 0; i < people_count && !paused; i++) {
            /* Update infected stats */
            if(!A.alive) {
                dead_count[graph_instant]++;
                A.vel.x = 0; A.vel.y = 0;
            }
            if(A.alive && A.immune) immune_count[graph_instant]++;
            if(!A.immune && A.infected) infected_count[graph_instant]++;

            /* Heal or die */
            if (A.alive && A.immune == 0 && A.infected > 0 && fc - A.infected >= healing_time) {
                if(RAND_FLOAT < mortality) {
                    A.alive = false;
                    A.immune = fc;
                    A.c = RED;
                } else {
                    A.immune = fc;
                    A.infected = 0;
                    A.c = GREEN;
                }
            }

            /* Update position */
            A.pos.x += A.vel.x;
            A.pos.y += A.vel.y;

            /* Random drifting  */
            if (A.alive && fc % 4 == 0 && A.vel.x != A.vel.y != 0) {
                float drift = RAND_FLOAT * 0.4 * (RAND_FLOAT < 0.5 ? 1 : -1);
                A.vel.x += drift;
                A.vel.y -= drift;
            }

            /* Bounce off the screen bounds */
            if((A.pos.x >= (GetScreenWidth() - br)) || (b[i].pos.x <= br))
                A.vel.x *= -1;
            if((A.pos.y >= (GetScreenHeight() - br - graph_height)) || (b[i].pos.y <= br))
                A.vel.y *= -1;

            for(j = i; j < people_count; j++) {
                if(j == i) continue;
                if (CheckCollisionCircles(A.pos, br, B.pos, br)) {
                    A.vel.x *= -1; A.vel.y *= -1;
                    B.vel.x *= -1; B.vel.y *= -1;

                    /* Infect */
                    if (A.alive && B.alive
                        && (A.infected > 0 || B.infected > 0)
                        && A.immune == 0
                        && B.immune == 0
                        && RAND_FLOAT < infectivity) {
                        b[j].infected = fc;
                        b[j].c = INFECTED;
                        b[i].infected = fc;
                        b[i].c = INFECTED;
                    }
                }
            }
        }

        /* Drawing Loop */
        BeginDrawing();
            ClearBackground((Color){0,0,0,255});
            for (i = 0; i < people_count; i++) {
                DrawCircle(b[i].pos.x, b[i].pos.y, br, b[i].c);
            }

            sprintf(msg, "Total balls: %d    Death chance: %f\nInfected: %d    Infection chance: %f\nRecovered: %d    Dead: %d\nRemaining time: %.2fs\nElapsed time: %.2fs    1/%d of people are moving",
                people_count, mortality,
                infected_count[graph_instant], infectivity,
                immune_count[graph_instant],
                dead_count[graph_instant],
                maxtime - (float)fc/fps, (float)fc/fps, not_staying_home);

            DrawText(msg,
                graph_max_width + 5,
                GetScreenHeight() - graph_height,
                14,
                WHITE);

            for(i = 0; i < graph_instant && i < graph_width; i++) {
                bar_width = graph_max_width/graph_instant;
                bar_dead_height = (dead_count[i] * graph_height) / people_count;
                bar_immune_height = (immune_count[i] * graph_height) / people_count;
                bar_infected_height = (infected_count[i] * graph_height) / people_count;
                DrawRectangle(bar_width * i, GetScreenHeight() - graph_height, bar_width, graph_height, GRAY);
                DrawRectangle(bar_width * i, GetScreenHeight() - (bar_infected_height + bar_immune_height + bar_dead_height), bar_width, bar_infected_height, PURPLE);
                DrawRectangle(bar_width * i, GetScreenHeight() - (bar_immune_height + bar_dead_height), bar_width, bar_immune_height, GREEN);
                DrawRectangle(bar_width * i, GetScreenHeight() - bar_dead_height, bar_width, bar_dead_height, RED);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
