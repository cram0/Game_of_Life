#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

size_t const WIDTH = 100, HEIGHT = 100;
int const W_WIDTH = 800, W_HEIGHT = 800;

int cell_width = (W_WIDTH - 1) / WIDTH;
int cell_height = (W_HEIGHT - 1) / HEIGHT;

clock_t gameClock;

int running = 1;

SDL_Event event;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Rect cell_rectangle = { 0, 0, 0, 0 };

bool grid_activated[HEIGHT][WIDTH];

int getNumberOfLivingCellAround(bool map_activated[][WIDTH], int x, int y, int max_x, int max_y)
{
    int count = 0;

    if (x - 1 >= 0) {

        if (y - 1 >= 0) {
            map_activated[y - 1][x - 1] == true ? count++ : false;
        }

        map_activated[y][x - 1] == true ? count++ : false;
        if (y + 1 <= max_y) {

            map_activated[y + 1][x - 1] == true ? count++ : false;
        }
    }
    if (x + 1 <= max_x) {
        if (y - 1 >= 0) {
            map_activated[y - 1][x + 1] == true ? count++ : false;
        }

        map_activated[y][x + 1] == true ? count++ : false;

        if (y + 1 <= max_y) {
            map_activated[y + 1][x + 1] == true ? count++ : false;
        }
    }
    if (y - 1 >= 0) {
        map_activated[y - 1][x] == true ? count++ : false;
    }
    if (y + 1 <= max_y) {
        map_activated[y + 1][x] == true ? count++ : false;
    }

    return (count);
}

double getElapsedTime(clock_t* _clock)
{
    return ((double)(clock() - *_clock) / CLOCKS_PER_SEC);
}

void restartClock(clock_t* _clock)
{
    *_clock = clock();
}

void mainLoop(void)
{
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }
    }

    if (getElapsedTime(&gameClock) * 1000 >= 15) {
        bool new_grid[HEIGHT][WIDTH] = { false };

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                int number = getNumberOfLivingCellAround(grid_activated, x, y, WIDTH, HEIGHT);
                if (grid_activated[y][x] == true) {
                    if (number == 2 || number == 3) {
                        new_grid[y][x] = true;
                    } else
                        new_grid[y][x] = false;
                } else if (grid_activated[y][x] == false) {
                    if (number == 3) {
                        new_grid[y][x] = true;
                    }
                }

                if (new_grid[y][x] == true) {
                    SDL_SetRenderDrawColor(renderer, (Uint8) { 255 }, (Uint8) { 255 }, (Uint8) { 255 }, (Uint8) { 255 });
                } else if (new_grid[y][x] == false) {
                    SDL_SetRenderDrawColor(renderer, (Uint8) { 0 }, (Uint8) { 0 }, (Uint8) { 0 }, (Uint8) { 255 });
                }
                SDL_Rect cell_rectangle = { x * (cell_width + 1), y * (cell_height + 1), cell_width, cell_height };
                SDL_RenderDrawRect(renderer, &cell_rectangle);
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);

        for (size_t y = 0; y < HEIGHT; y++) {
            for (size_t x = 0; x < WIDTH; x++) {
                grid_activated[y][x] = new_grid[y][x];
            }
        }

        restartClock(&gameClock);
    }
}

int main(void)
{
    srand(time(NULL));

    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_PRESENTVSYNC;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W_WIDTH, W_HEIGHT, windowFlags);

    if (!window) {
        printf("Failed to open %d x %d window: %s\n", W_WIDTH, W_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    if (!renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Color color = { 230, 230, 230, 255 };

    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            grid_activated[y][x] = false;
        }
    }

    int cell_pos_y = 0;
    for (size_t y = 0; y < HEIGHT; y++) {
        int cell_pos_x = 0;
        for (size_t x = 0; x < WIDTH; x++) {
            grid_activated[y][x] = (rand() % (100 - 0 + 1) <= 50 ? true : false);

            SDL_Point cell_pos = { cell_pos_x, cell_pos_y };
            if (grid_activated[y][x] == true) {
                SDL_SetRenderDrawColor(renderer, (Uint8) { 255 }, (Uint8) { 255 }, (Uint8) { 255 }, (Uint8) { 255 });
            } else {
                SDL_SetRenderDrawColor(renderer, (Uint8) { 0 }, (Uint8) { 0 }, (Uint8) { 0 }, (Uint8) { 255 });
            }

            SDL_Rect cell_rectangle = { cell_pos_x, cell_pos_y, cell_width, cell_height };
            SDL_RenderDrawRect(renderer, &cell_rectangle);

            cell_pos_x += cell_width + 1;
        }
        cell_pos_y += cell_height + 1;
    }

    gameClock = clock();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while (running) {
        mainLoop();
    }
#endif

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);

    return (0);
}