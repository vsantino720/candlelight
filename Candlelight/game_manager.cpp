// includes
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <SDL3/SDL.h>
#include "clobject.h"
#include "game_manager.h"
#include "game_renderer.h"
#include "input_manager.h"
#include "tilemap.h"

// singleton initialization
c_game_manager* c_game_manager::m_instance = nullptr;

// public methods

// Static singletone instantiation methods
c_game_manager* c_game_manager::try_to_initialize_game(
	c_game_renderer* game_renderer,
	c_input_manager* input_context,
	s_tilemap* initial_tilemap)
{
	if (m_instance == nullptr)
	{
		m_instance = new c_game_manager();
	}

	m_instance->set_input_context(input_context);
	m_instance->set_game_renderer(game_renderer);
	m_instance->set_current_tilemap(initial_tilemap);

	return m_instance;
}

// Main game loop. Returns true if the game ran successfully, false otherwise.
bool c_game_manager::run_game()
{
	assert(m_game_renderer != nullptr);
	assert(m_input_context != nullptr);
	assert(m_current_tilemap != nullptr);

	bool result = true;

	result = m_game_renderer->initialize_window();

	if (result)
	{
		bool running = true;

		// $TODO: Explore removing the SDL dependency within the game manager
		Uint64 previous_ticks = SDL_GetPerformanceCounter();
		const Uint64 frequency = SDL_GetPerformanceFrequency();

		while (running) 
		{
			Uint64 current_ticks = SDL_GetPerformanceCounter();
			m_delta_time = (double)(current_ticks - previous_ticks) / (double)frequency;
			previous_ticks = current_ticks;

			// --- Input ---
			{
				m_input_context->update();

				running = m_input_context->event_was_fired(QUIT_GAME_EVENT_NAME)
					? false
					: true;
			}

			// --- Update ---
			{
				update_objects();
			}

			// --- Render ---
			{
				m_game_renderer->update();
				m_game_renderer->draw_tilemap_from_viewpoint(
					*m_current_tilemap,
					{ 8.0f, 4.0f }); // $TODO: Add camera support
				m_game_renderer->render();
			}
		}
	}

	return result;
}

const c_game_manager& c_game_manager::get_instance()
{
	assert(m_instance != nullptr);
	return *m_instance;
}

const c_input_manager& c_game_manager::get_readonly_input_manager() const
{
	return const_cast<const c_input_manager&>(get_input_context());
}

c_input_manager& c_game_manager::get_input_context() const
{
	assert(m_input_context != nullptr);
	return *m_input_context;
}

void c_game_manager::set_current_tilemap(s_tilemap* tilemap)
{
	m_current_tilemap = tilemap;
}

void c_game_manager::set_input_context(c_input_manager* input_context)
{
	m_input_context = input_context;
}

void c_game_manager::set_game_renderer(c_game_renderer* game_renderer)
{
	m_game_renderer = game_renderer;
}

// private methods

void c_game_manager::update_objects()
{
	// Update the current tilemap and object positions
	if (m_current_tilemap != nullptr)
	{
		int max_tiles_x = m_current_tilemap->max_tiles_x;
		int max_tiles_y = m_current_tilemap->max_tiles_y;

		for (int x = 0; x < max_tiles_x; ++x)
		{
			for (int y = 0; y < max_tiles_y; ++y)
			{
				c_clobject* obj = m_current_tilemap->m_tiles[x][y];

				if (obj != nullptr)
				{
					// Run the update function for the object, which will in turn update its attributes
					obj->update();

					// Check if the object has moved to a new tile, and enforce collision and tilebounds
					sanitize_object_position(obj, { x, y });
				}
			}
		}
	}
}

// Modifies the tile offset to guarantee it fits within the tile move threshold
void c_game_manager::sanitize_tile_offset(t_tile_position* offset)
{
	assert(offset != nullptr);

	offset->x = (offset->x < -TILE_MOVE_THRESHOLD) ? -TILE_MOVE_THRESHOLD : offset->x;
	offset->x = (offset->x > TILE_MOVE_THRESHOLD) ? TILE_MOVE_THRESHOLD : offset->x;
	offset->y = (offset->y < -TILE_MOVE_THRESHOLD) ? -TILE_MOVE_THRESHOLD : offset->y;
	offset->y = (offset->y > TILE_MOVE_THRESHOLD) ? TILE_MOVE_THRESHOLD : offset->y;
}

// Migrates the tile offset across tile boundary lines
void c_game_manager::migrate_tile_offset(
	t_tile_position* offset, 
	const t_tilemap_position& old_pos, 
	const t_tilemap_position& new_pos)
{
	assert(offset != nullptr);

	const s_vector<float> old_tilemap_position = { old_pos.x + offset->x, old_pos.y + offset->y };
	const s_vector<float> new_tile_tilemap_position = { (float)new_pos.x, (float)new_pos.y };

	// Calculates distance to center point of new tile relative to the new tile center
	*offset = old_tilemap_position - new_tile_tilemap_position;
}

void c_game_manager::sanitize_object_position(c_clobject* obj, const t_tilemap_position& pos)
{
	int max_tiles_x = m_current_tilemap->max_tiles_x;
	int max_tiles_y = m_current_tilemap->max_tiles_y;

	t_tile_position offset = obj->get_relative_tile_offset();

	if (offset_exceeded_tile_threshold(offset))
	{
		// $TODO: Handle case where speed is fast enough that we actually cross multiple tile borders in one frame???
		// i.e. teleportation support? How do we resolve collisions? Remain at the original spot or find adjacent free spot?

		int new_x = round_to_nearest_tile(pos.x + offset.x);
		int new_y = round_to_nearest_tile(pos.y + offset.y);

		// If either exceeds the maximum tilemap bounds, we will clamp
		// the position to the maximum tilemap bounds.
		new_x = std::clamp(new_x, 0, max_tiles_x - 1);
		new_y = std::clamp(new_y, 0, max_tiles_y - 1);

		const bool new_position_empty = m_current_tilemap->m_tiles[new_x][new_y] == nullptr;

		// Check if the new position is within bounds
		if (new_position_empty)
		{
			// Move the object to the new tile
			m_current_tilemap->m_tiles[new_x][new_y] = obj;
			m_current_tilemap->m_tiles[pos.x][pos.y] = nullptr;

			// Migrate offset across tile boundary
			migrate_tile_offset(&offset, pos, { new_x, new_y });
		}
		else // Sanitize offset to fit within appropriate tile bounds
		{
			sanitize_tile_offset(&offset);
		}

		obj->set_relative_tile_offset(offset);
	}
}

int c_game_manager::round_to_nearest_tile(float value) const
{
	int result = static_cast<int>(value);

	float fraction = fmod(value, 1);

	if (fraction > TILE_MOVE_THRESHOLD)
	{
		result = std::ceil(value);
	}
	else if (fraction < -TILE_MOVE_THRESHOLD)
	{
		result = std::floor(value);
	}

	return result;
}
