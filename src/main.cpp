#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include "Deck.h"
#include "CombatState.h"
#include "UIRenderer.h"

int main(int argc, char* argv[]) {
#if defined(_WIN32) || defined(_WIN64)
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
#else
    // Linux/Wayland/X11
    SDL_SetHint(SDL_HINT_VIDEODRIVER, "x11");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
#endif

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
        return 1;
    }

    std::string basePath;
#if defined(_WIN32) || defined(_WIN64)
    char* path = SDL_GetBasePath();
    if (path) {
        basePath = path;
        SDL_free(path);
    } else {
        basePath = ".\\";
    }
#else
    char* path = SDL_GetBasePath();
    basePath = path ? path : "./";
    if (path) SDL_free(path);
#endif

    TTF_Font* font = TTF_OpenFont((basePath + "../src/fonts/Monsterrat.ttf").c_str(), 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
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

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        std::cerr << "Renderer failed: " << SDL_GetError() << "\n";
        return 1;
    }

    CombatState state {
        { 30, 30 }, // HP
        { 3, 3 } // Mana
    };

    UIRenderer ui;
    Deck deck;
    deck.shuffle();
    deck.drawInitialHand();

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);

        ui.render(renderer, state, winW, winH, font);
        deck.render(renderer, winW, winH);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    SDL_Quit();
    TTF_Quit();

    return 0;
}

