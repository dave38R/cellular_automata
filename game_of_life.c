#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int CELL_SIZE = 10;

// Grid dimensions
const int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

void initializeGrid(int grid[GRID_WIDTH][GRID_HEIGHT]);
void drawGrid(SDL_Renderer *renderer, int grid[GRID_WIDTH][GRID_HEIGHT]);
void updateGrid(int grid[GRID_WIDTH][GRID_HEIGHT]);

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create an SDL window
    SDL_Window *win = SDL_CreateWindow("Conway's Game of Life", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
        updateGrid(grid);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the grid
        drawGrid(renderer, grid);

        // Present the renderer
        SDL_RenderPresent(renderer);

        // Delay to control the speed of the simulation
        SDL_Delay(100);
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

void initializeGrid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    srand(time(NULL));
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            grid[x][y] = rand() % 2;  // Randomly set cells to alive (1) or dead (0)
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

void updateGrid(int grid[GRID_WIDTH][GRID_HEIGHT]) {
    int newGrid[GRID_WIDTH][GRID_HEIGHT];
    // Initialize newGrid to all zeros
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            newGrid[x][y] = 0;
        }
    }

    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            int aliveNeighbors = 0;

            // Count alive neighbors
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT) {
                        aliveNeighbors += grid[nx][ny];
                    }
                }
            }

            // Apply the rules of Conway's Game of Life
            if (grid[x][y] == 1) {
                if (aliveNeighbors < 2 || aliveNeighbors > 3) {
                    newGrid[x][y] = 0;  // Cell dies
                } else {
                    newGrid[x][y] = 1;  // Cell lives
                }
            } else {
                if (aliveNeighbors == 3) {
                    newGrid[x][y] = 1;  // Cell becomes alive
                }
            }
        }
    }

    // Copy new grid to the original grid
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            grid[x][y] = newGrid[x][y];
        }
    }
}
