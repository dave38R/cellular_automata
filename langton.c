#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int CELL_SIZE = 10;

// Grid dimensions
const int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

// Ant structure
typedef struct{
    int x;
    int y;
    int direction[2];
}Ant;

void *turn(Ant *ant, bool right);
void initializeGrid(int grid[GRID_WIDTH][GRID_HEIGHT], Ant *ant);
void drawGrid(SDL_Renderer *renderer, int grid[GRID_WIDTH][GRID_HEIGHT], Ant *ant);
void updateGrid(int grid[GRID_WIDTH][GRID_HEIGHT], Ant *ant);

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create an SDL window
    SDL_Window *win = SDL_CreateWindow("Langton's Ant", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create an SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialize the grid
    int grid[GRID_WIDTH][GRID_HEIGHT];
    initializeGrid(grid);

    // Initialize the ant
    Ant ant = {(int)GRID_WIDTH/2, (int)GRID_HEIGHT/2, {0, 1}};

    // Main loop
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Update the grid
        updateGrid(grid, &ant);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the grid
        drawGrid(renderer, grid);

        // Present the renderer
        SDL_RenderPresent(renderer);

        // Delay to control the speed of the simulation
        // SDL_Delay(1);
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

void *turn(Ant *ant, bool right){
    // Right turn matrix
    int rT[2][2] = {{0, 1}, {-1, 0}};
    // Left turn matrix
    int lT[2][2] = {{0, -1}, {1, 0}};
    // Initialize the new directions
    int new_direction[2] = {0, 0};
    // Calculate the new directions
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            new_direction[i] += right ? ant->direction[j]*rT[i][j] : ant->direction[j]*lT[i][j];
        }
    }
    // Assign the new direction to the ant's direction
    ant->direction[0] = new_direction[0];
    ant->direction[1] = new_direction[1];
}


void initializeGrid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    srand(time(NULL));
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            grid[x][y] = 0;  // All cells are initially dead
        }
    }
}

void drawGrid(SDL_Renderer *renderer, int grid[GRID_WIDTH][GRID_HEIGHT]) {
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            if (grid[x][y] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Alive cells are white
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Dead cells are black
            }
            SDL_Rect cell = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            SDL_RenderFillRect(renderer, &cell);
        }
    }
}

void updateGrid(int grid[GRID_WIDTH][GRID_HEIGHT], Ant *ant) {
    // Apply the rules of Langton's Ant
    // get current tile's color
    int color = grid[ant->x][ant->y];
    // make the ant turn accordingly
    (color == 0) ? turn(ant, true) : turn(ant, false);
    // change the tile's color
    grid[ant->x][ant->y] = (color +1)%2;
    // move the ant to the next tile
    ant->x += ant->direction[0];
    ant->y += ant->direction[1];
}
