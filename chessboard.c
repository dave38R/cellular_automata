// C program to create a chess board using SDL2
#include <SDL2/SDL.h>
#include <stdio.h>

// Window dimensions
const int WINDOW_WIDTH = 480;
const int WINDOW_HEIGHT = 480;
const int RECT_SIZE = 60;

// Function prototypes
void drawChessBoard(SDL_Renderer *renderer);

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create an SDL window
    SDL_Window *win = SDL_CreateWindow("Chess Board", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

    // Draw the chess board
    drawChessBoard(renderer);

    // Show the window for a while
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_Delay(30);
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

void drawChessBoard(SDL_Renderer *renderer) {
    int black = 0;
    SDL_Rect rect;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            rect.x = c * RECT_SIZE;
            rect.y = r * RECT_SIZE;
            rect.w = RECT_SIZE;
            rect.h = RECT_SIZE;

            if (black) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
            }
            SDL_RenderFillRect(renderer, &rect);

            black = !black;
        }
        black = !black;
    }

    SDL_RenderPresent(renderer);
}
