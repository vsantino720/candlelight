// includes
#include <assert.h>
#include <iostream>
#include "clobject.h"
#include "game_renderer.h"
#include "tilemap.h"
#include "vector.h"

// Initialize game window and any other necessary setup.
bool c_game_renderer::initilize_window()
{
    // SDL3 functions return bool now (true = success) instead of SDL2's
    // "0 or positive = success" convention.
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // SDL3's SDL_CreateWindow dropped the x/y position parameters SDL2 had;
    // windows are positioned by the platform by default. Use
    // SDL_SetWindowPosition() afterward if you need explicit placement.
    m_window = SDL_CreateWindow(
        m_window_name.c_str(),
        m_window_width, m_window_height,
        SDL_WINDOW_RESIZABLE);

    if (!m_window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // SDL3's SDL_CreateRenderer takes a driver name (or nullptr for the
    // default) instead of SDL2's numeric index + flags bitmask.
    m_renderer = SDL_CreateRenderer(m_window, nullptr);

    if (!m_renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    // VSync is now a separate call rather than a renderer creation flag.
    SDL_SetRenderVSync(m_renderer, 1);

    return true;
}

void c_game_renderer::draw_object(const c_clobject& obj, const t_tilemap_position& pos)
{
    assert(m_renderer != nullptr);

    const t_tile_position offset = obj.get_relative_tile_offset();

    // A filled rectangle - stand-in for a tile/sprite
    // until texture loading is wired up in Phase 2.
    SDL_FRect filledRect
    {
        ((pos.x + offset.x) * OBJECT_SIZE),
        ((pos.y + offset.y) * OBJECT_SIZE),
        (float)OBJECT_SIZE,
        (float)OBJECT_SIZE
    };

    SDL_SetRenderDrawColor(m_renderer, 220, 60, 60, 255);
    SDL_RenderFillRect(m_renderer, &filledRect);
}

void c_game_renderer::draw_tile(const t_tilemap_position& pos)
{
    assert(m_renderer != nullptr);

    // A filled rectangle, roughly centered - stand-in for a tile/sprite
    // until texture loading is wired up in Phase 2.
    SDL_FRect outlinedRect
    {
        (pos.x * TILE_SIZE),
        (pos.y * TILE_SIZE),
        (float)TILE_SIZE,
        (float)TILE_SIZE
    };

    SDL_SetRenderDrawColor(m_renderer, 90, 200, 120, 255);
    SDL_RenderRect(m_renderer, &outlinedRect);
}

// $TODO: Support colors, textures, etc.
void c_game_renderer::draw_background()
{
    assert(m_renderer != nullptr);

    SDL_SetRenderDrawColor(m_renderer, 24, 24, 32, 255);
    SDL_RenderClear(m_renderer);
}

void c_game_renderer::render()
{
    SDL_RenderPresent(m_renderer);
}

c_game_renderer::~c_game_renderer()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}