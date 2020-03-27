#include <stdlib.h>
#include <raylib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <getopt.h>

#define RAND_RANGE(min, max) \
    (random() % (max + 1 - min)) + min

#define RAND_FLOAT (float)rand()/RAND_MAX
#define RAND_SIGN (RAND_RANGE(0, 1) == 0 ? -1 : 1)

#define CLEANUP(fp) if(ofp) fclose(fp);

#define BACKGROUND 0x1C1B19FF

typedef enum {
    DEAD = 0xEF2F27FF,
    RECOVERED = 0x519F50FF,
    INFECTED = 0x2C78BFFF,
    SUSCEPTIBLE = 0xFCE8C3FF
} state_t;

typedef struct __cell {
    Vector2 pos;
    Vector2 vel;
    state_t state;
    /* Frame of infection */
    long int infection_time;
    /* Radius */
    int r;
    /* boolean variable if cell is alive */
    bool alive;
    /* Is quarantined */
    bool quarantined;
} cell_t;

cell_t random_cell(cell_t a) {
    a.r = 3;
    a.pos = (Vector2) {
        RAND_RANGE(a.r, GetScreenWidth() - a.r),
        RAND_RANGE(a.r, GetScreenHeight() - a.r)
    };
    a.vel = (Vector2){ RAND_RANGE(1,3), RAND_RANGE(1, 3) };
    a.state = SUSCEPTIBLE;
    a.alive = true;
    a.infection_time = 0;
    return a;
}

typedef struct __options {
    /* Max simulation time in seconds */
    int maxtime;
    /* Frames per second */
    int fps;
    /* Screen width and height */
    int screen_w;
    int screen_h;
    /* Total population */
    int population;
    /* number of quarantined people */
    int total_quarantined;
    /* Probability of infecting another individual */
    double infectivity;
    /* Probability of death */
    double mortality;
    /* Healing time in frames */
    double healing_time;
    /* How many frames per sample */
    int sampling_rate;
    /* Maximum Speed */
    float max_speed;
    /* Output filename */
    char* output_filename;
} options_t;

const options_t default_options = {
    40,         /* maxtime */
    60,         /* FPS */
    1280,       /* screen_w */
    720,        /* screen_h */
    300,        /* population */
    300/4,      /* quarantined */
    0.57,       /* infectivity */
    0.1,        /* mortality */
    5*60,       /* healing_time */
    5,          /* sampling_rate */
    5.0,          /* max_speed */
    "output.csv"
};

void print_usage(const char* name) {
    fprintf(stderr, "Usage: %s -t MAXTIME -f FPS -w WIDTH -h HEIGHT\n"
        "\t-p POPULATION -q QUARANTINED -i INFECTIVITY\n"
        "\t-m MORTALITY -a HEALING_TIME -s SAMPLERATE\n"
        "\t-z MAXSPEED", name);
    exit(EXIT_FAILURE);
}

options_t get_options(int argc, char* const argv[]) {
    options_t o = default_options;
    int c;
    while ((c = getopt (argc, argv, "t:f:w:h:p:q:i:m:h:s:o:")) != -1)
    switch (c) {
        case 't': o.maxtime = atoi(optarg); break;
        case 'f': o.fps = atoi(optarg); break;
        case 'w': o.screen_w = atoi(optarg); break;
        case 'h': o.screen_h = atoi(optarg); break;
        case 'p': o.population = atoi(optarg); break;
        case 'q': o.total_quarantined = atoi(optarg); break;
        case 'i': o.infectivity = atof(optarg); break;
        case 'm': o.mortality = atof(optarg); break;
        case 'z': o.max_speed = atof(optarg); break;
        case 'a': o.healing_time = atoi(optarg); break;
        case 's': o.sampling_rate = atoi(optarg); break;
        case 'o': o.output_filename = optarg; break;
        default:
            print_usage(argv[0]);
    }
    return o;
}

/* Open CSV file */
FILE* open_csv(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if(fp == NULL) {
        fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fp;
}


int main(int argc, char* const argv[]) {
    options_t o = get_options(argc, argv);

    int i = 0, j = 0, paused = 0, first_infected = 0;
    int infected_count = 0, recovered_count = 0, dead_count = 0;
    long long int fc = 0;
    long int graph_instant = 0;
    double elapsed_time = 0.0, elapsed_pause_time = 0.0;
    cell_t *b = malloc(sizeof(cell_t) * o.population);
    char *msg = malloc(2048);

    /* Init random number generator */
    srandom(time(NULL));

    /* Init the window */
    InitWindow(o.screen_w, o.screen_h, "demo");
    SetTargetFPS(o.fps);

    /* Quarantine some cells */
    for (i = 0; i < o.population; i++) {
        b[i] = random_cell(b[i]);
        if(j < o.total_quarantined) {
            b[i].quarantined = true;
            j++;
        }
    }

    for(int i = 0; i < 4; i++) {
        int j = i;
        /* single random cell is infected */
        while(b[i].quarantined || b[i].state == INFECTED) i++;
        b[i].state = INFECTED;
        b[i].infection_time = 1;
        infected_count++;
    }

    /* Init output file */
    FILE* ofp = open_csv(o.output_filename);
    fprintf(ofp, "Time, Susceptible, Infected, Recovered, Dead\n");

    /* SIMULATION LOOP */
    while(!WindowShouldClose()) {
        if(fc >= o.maxtime*o.fps) paused = true;

        // Custom pause
        if (IsKeyPressed(' ') && fc <= o.maxtime * o.fps) {
            paused = !paused;
        }

        /* Update time and CSV if not paused */
        if(!paused) {
            fc++;
            elapsed_time = GetTime() - elapsed_pause_time;
            if(graph_instant != fc/o.sampling_rate) {
                graph_instant = fc/o.sampling_rate;
                fprintf(ofp, "%ld,%d,%d,%d,%d\n", graph_instant, (o.population - infected_count - recovered_count - dead_count), infected_count, recovered_count, dead_count);
                fflush(ofp);
            }
        } else {
            elapsed_pause_time = GetTime() - elapsed_time;
        }

        /* Updating Loop */
        for (i = 0; i < o.population && !paused; i++) {

            /* Heal or die */
            if (b[i].alive && b[i].state == INFECTED && fc - b[i].infection_time >= o.healing_time) {
                if(RAND_FLOAT < o.mortality) {
                    b[i].alive = false;
                    b[i].state = DEAD;
                    dead_count++;
                    infected_count--;
                } else {
                    b[i].state = RECOVERED;
                    infected_count--;
                    recovered_count++;
                }
            }

            /* Update position */
            if(!b[i].quarantined && b[i].alive) {
                b[i].pos.x += b[i].vel.x;
                b[i].pos.y += b[i].vel.y;
                /* Random drifting */
                float drift = RAND_FLOAT * 0.4 * (RAND_FLOAT < 0.5 ? 1 : -1);
                b[i].vel.x += drift * RAND_SIGN;
                b[i].vel.y += drift * RAND_SIGN;
                if (b[i].vel.x > o.max_speed) b[i].vel.x = o.max_speed;
                if (b[i].vel.x < -o.max_speed) b[i].vel.x = -o.max_speed;
                if (b[i].vel.y > o.max_speed) b[i].vel.y = o.max_speed;
                if (b[i].vel.y < -o.max_speed) b[i].vel.y = -o.max_speed;

            }


            /* Wrap off the screen bounds */
            if((b[i].pos.x >= (GetScreenWidth() + b[i].r)) || (b[i].pos.x <= -b[i].r))
                b[i].pos.x = GetScreenWidth() - b[i].pos.x;
            if((b[i].pos.y >= (GetScreenHeight() + b[i].r)) || (b[i].pos.y <= -b[i].r))
                b[i].pos.y = GetScreenHeight() - b[i].pos.y;


            for(j = i; j < o.population; j++) {
                if(j == i) continue;
                if (CheckCollisionCircles(b[i].pos, b[i].r, b[j].pos, b[i].r)) {
                    b[i].vel.x *= -1; b[i].vel.y *= -1;
                    b[j].vel.x *= -1; b[j].vel.y *= -1;

                    /* Infect */
                    if ((b[i].alive == b[j].alive) == true) {
                        if (b[i].state == INFECTED && b[j].state == SUSCEPTIBLE) {
                            if(RAND_FLOAT < o.infectivity) {
                                b[j].infection_time = fc;
                                b[j].state = INFECTED;
                                infected_count++;
                            }
                        } else if (b[j].state == INFECTED && b[i].state == SUSCEPTIBLE) {
                            if(RAND_FLOAT < o.infectivity) {
                                b[i].infection_time = fc;
                                b[i].state = INFECTED;
                                infected_count++;
                            }
                        }
                    }
                }
            }
        }

        /* Drawing Loop */
        BeginDrawing();
            ClearBackground(GetColor(BACKGROUND));
            for (i = 0; i < o.population; i++) {
                DrawCircle(b[i].pos.x, b[i].pos.y, b[i].r, GetColor(b[i].state));
            }

            sprintf(msg, "Total cells: %d    Death chance: %f    FPS: %d\n"
            "Infected: %d    Infection chance: %f\n"
            "Recovered: %d    Dead: %d\n"
            "Remaining time: %.2fs\n"
            "Elapsed time: %.2fs    %d quarantined",
                o.population, o.mortality, o.fps,
                infected_count, o.infectivity,
                recovered_count,
                dead_count,
                o.maxtime - elapsed_time, elapsed_time, o.total_quarantined);

            DrawText(msg, 16, 16, 14, WHITE);
        EndDrawing();
    }

    free(msg);
    CLEANUP(ofp);

    CloseWindow();
    return 0;
}
