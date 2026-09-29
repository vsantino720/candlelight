// main.cpp - Minimal SDL3 window + game loop
// This is Phase 1 of the engine: prove the toolchain works end to end
// (CMake -> vcpkg -> SDL3 -> window -> render loop) before any
// game-specific systems get built on top of it.

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // SDL3 functions return bool now (true = success) instead of SDL2's
    // "0 or positive = success" convention.
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // SDL3's SDL_CreateWindow dropped the x/y position parameters SDL2 had;
    // windows are positioned by the platform by default. Use
    // SDL_SetWindowPosition() afterward if you need explicit placement.
    SDL_Window* window = SDL_CreateWindow(
        "Candlelight",
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // SDL3's SDL_CreateRenderer takes a driver name (or NULL for the
    // default) instead of SDL2's numeric index + flags bitmask.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // VSync is now a separate call rather than a renderer creation flag.
    SDL_SetRenderVSync(renderer, 1);

    bool running = true;
    SDL_Event event;

    Uint64 previousTicks = SDL_GetPerformanceCounter();
    const Uint64 frequency = SDL_GetPerformanceFrequency();

	TilePosition currentPlayerPosition{ MAX_TILES_X / 2, MAX_TILES_Y / 2 };
	TilePosition targetPlayerPosition{ currentPlayerPosition.x, currentPlayerPosition.y };

    while (running) {
        Uint64 currentTicks = SDL_GetPerformanceCounter();
        double deltaTime = (double)(currentTicks - previousTicks) / (double)frequency;
        previousTicks = currentTicks;

        // --- Input ---
        while (SDL_PollEvent(&event)) {
            // Event type constants gained an SDL_EVENT_ prefix in SDL3
            // (SDL_QUIT -> SDL_EVENT_QUIT, SDL_KEYDOWN -> SDL_EVENT_KEY_DOWN).
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            // The nested event.key.keysym.sym from SDL2 is gone; SDL3
            // flattens it to event.key.key directly.
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                running = false;
            }
        }

        // --- Update ---
        // TODO: world/state update goes here, driven by deltaTime.
        (void)deltaTime;

        // --- Render ---
        SDL_SetRenderDrawColor(renderer, 24, 24, 32, 255);
        SDL_RenderClear(renderer);

        // A filled rectangle, roughly centered - stand-in for a tile/sprite
        // until texture loading is wired up in Phase 2.
        SDL_FRect filledRect
        {
            playerPosition.x - (TILE_SIZE / 2.0f),
            playerPosition.y - (TILE_SIZE / 2.0f),
            (float)TILE_SIZE,
            (float)TILE_SIZE
        };

        SDL_SetRenderDrawColor(renderer, 220, 60, 60, 255);
        SDL_RenderFillRect(renderer, &filledRect);

        // TODO: draw tilemap, entities, UI here.

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}