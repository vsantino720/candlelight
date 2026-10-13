// includes
#include <assert.h>
#include <iostream>
#include "clobject.h"
#include "game_renderer.h"
#include "tilemap.h"
#include "vector.h"

// Initialize game window and any other necessary setup.
bool c_game_renderer::initialize_window()
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

void c_game_renderer::update()
{
	// Update the window size in case it has changed due to user resizing
	// $TODO: Consider caching the window size and only updating on window resize events
	SDL_GetWindowSize(m_window, &m_window_width, &m_window_height);
}

void c_game_renderer::draw_tilemap_from_viewpoint(const s_tilemap& tilemap, const t_tile_position& viewpoint)
{
	// $TODO: Draw background based upon the tilemap's properties (e.g., color, texture, etc.)
    draw_background();

	const int max_tiles_x = tilemap.max_tiles_x;
	const int max_tiles_y = tilemap.max_tiles_y;

#ifdef DEBUG_ENABLED
    // For each tile, determine screen-space position and draw it if it's within the viewable area.
    for (int x = 0; x < max_tiles_x; ++x)
    {
        for (int y = 0; y < max_tiles_y; ++y)
        {
            const t_tilemap_position tile_pos = { x, y };

            s_vector<float> tile_screen_pos =
                tilemap_to_screen_tile_position(
                    {
                        (float)tile_pos.x,
                        (float)tile_pos.y
                    },
                    viewpoint);

            try_to_draw_tile(tile_screen_pos);
        }
    }
#endif

    for (int x = 0; x < max_tiles_x; ++x)
    {
        for (int y = 0; y < max_tiles_y; ++y)
        {
            const c_clobject* obj = tilemap.m_tiles[x][y];

            if (obj != nullptr)
            {
                const t_tilemap_position tile_pos = { x, y };

                t_tile_position tile_offset = obj->get_relative_tile_offset();

                s_vector<float> obj_screen_pos =
                    tilemap_to_screen_tile_position(
                        {
                            (float)tile_pos.x + tile_offset.x,
                            (float)tile_pos.y + tile_offset.y
                        },
                        viewpoint);

                try_to_draw_object(*obj, obj_screen_pos);
            }
        }
	}
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

// private methods

t_screen_tile_position c_game_renderer::tilemap_to_screen_tile_position(
    const t_tile_position& tilemap_pos,
    const t_tile_position& viewpoint_pos) const
{
    // Calculate screen pos, in tile units, and 0-indexed
    // In terms of matrix transformations, we are performing:
    // 
    // Translate by (-pos.x, -pos.y) to center the view on the given position
    // Translate by (m_view_width / 2, m_view_height / 2) to center the view on the screen
    t_screen_tile_position screen_pos =
    {
        (tilemap_pos.x - viewpoint_pos.x + (m_view_width / 2.0f)),
        (tilemap_pos.y - viewpoint_pos.y + (m_view_height / 2.0f))
    };

    return screen_pos;
}

t_screen_position c_game_renderer::screen_tile_to_screen_position(
	const t_screen_tile_position& screen_tile_pos) const
{
	const t_screen_dimensions tile_size = get_tile_size();
	t_screen_position screen_pos =
	{
		(screen_tile_pos.x * tile_size.x),
		(screen_tile_pos.y * tile_size.y)
	};

	return screen_pos;
}

void c_game_renderer::try_to_draw_object(const c_clobject& obj, const t_screen_tile_position& pos)
{
    assert(m_renderer != nullptr);

    // Check if the obj is even visible, given the object's dimensions
    // If it is even partially visible, we still render it
    const t_screen_dimensions tile_size = get_tile_size();

    t_screen_dimensions object_dimensions = { OBJECT_SCALAR * tile_size.x, OBJECT_SCALAR * tile_size.y };

    if (texture_is_viewable(pos, object_dimensions))
    {
        t_screen_position screen_pos = screen_tile_to_screen_position(pos);

        // A filled rectangle - stand-in for a tile/sprite
        // until texture loading is wired up in Phase 2.
        SDL_FRect filledRect
        {
            (screen_pos.x) - (OBJECT_SCALAR * tile_size.x / 2.0f),
            (screen_pos.y) - (OBJECT_SCALAR * tile_size.y / 2.0f),
            OBJECT_SCALAR * tile_size.x,
            OBJECT_SCALAR * tile_size.y
        };

        SDL_SetRenderDrawColor(m_renderer, 220, 60, 60, 255);
        SDL_RenderFillRect(m_renderer, &filledRect);
    }
}

void c_game_renderer::try_to_draw_tile(const t_screen_tile_position& screen_pos)
{
    assert(m_renderer != nullptr);

    const t_screen_dimensions tile_size = get_tile_size();

    if (texture_is_viewable(screen_pos, tile_size))
    {
        SDL_FRect outlinedRect
        {
            ((screen_pos.x - 0.5f) * tile_size.x),
            ((screen_pos.y - 0.5f) * tile_size.y),
            (float)tile_size.x,
            (float)tile_size.y
        };

        SDL_SetRenderDrawColor(m_renderer, 90, 200, 120, 255);
        SDL_RenderRect(m_renderer, &outlinedRect);
    }
}

// $TODO: Support colors, textures, etc.
void c_game_renderer::draw_background()
{
    assert(m_renderer != nullptr);

    SDL_SetRenderDrawColor(m_renderer, 24, 24, 32, 255);
    SDL_RenderClear(m_renderer);
}