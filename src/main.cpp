#include <SDL2/SDL.h>
#include <iostream>
#include "Deck.h"

int main(int argc, char* argv[]) {
    SDL_SetHint(SDL_HINT_VIDEODRIVER, "x11");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    const int winW = 800;
    const int winH = 800;

    SDL_Window* window = SDL_CreateWindow(
        "Card Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        winW,
        winH,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (!renderer) {
        std::cerr << "Renderer failed\n";
        return 1;
    }

    Deck deck;
    deck.shuffle();
    deck.drawInitialHand();

    bool running = true;
    SDL_Event event;

    while (running) {
        // --- handle events ---
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // --- clear screen ---
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);

        // --- render deck ---
        deck.render(renderer, winW, winH);

        // --- present ---
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

