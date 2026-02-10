#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include "Deck/Deck.h"
#include "Other/CombatState.h"
#include "UI/UIRenderer.h"
#include "Enemy/Enemy.h"

int main(int argc, char* argv[]) {

  // get the correct software of the OS to be able to render without errors
#if defined(_WIN32) || defined(_WIN64)
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
#else
    SDL_SetHint(SDL_HINT_VIDEODRIVER, "x11");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
#endif

    // check if sld worked
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // check if ttf worked
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
        return 1;
    }

    // get the correct basepath of the OS
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

    // load font from correct path based on OS using the basepath
    TTF_Font* font = TTF_OpenFont((basePath + "../src/fonts/Monsterrat.ttf").c_str(), 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
        return 1;
    }

    // width and height of the game
    // maybe make it an array later for more reselutions?
    const int winW = 800;
    const int winH = 800;

    // spawn the window
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

    // set the players stats
    CombatState state {
        { 30, 30 }, // hp
        { 3, 3 }    // mana
    };

    UIRenderer ui;
    Deck deck;
    deck.shuffle();
    deck.drawInitialHand();

    Enemy enemy(10, 1);

    bool running = true;
    SDL_Event event;

    // game is running:
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);

        ui.render(renderer, state, winW, winH, font);
        deck.render(renderer, winW, winH);
        enemy.render(renderer, winW, winH);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // game is set to 60 fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    SDL_Quit();
    TTF_Quit();

    return 0;
}
