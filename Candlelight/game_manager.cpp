// includes
#include <assert.h>
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

	result = m_game_renderer->initilize_window();

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

				m_input_context->event_was_fired(QUIT_GAME_EVENT_NAME)
					? running = false
					: running = true;
			}

			// --- Update ---
			{
				update_objects();
			}

			// --- Render ---
			{
				m_game_renderer->draw_background();
				draw_tiles();
				draw_objects();
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

const c_input_manager& c_game_manager::get_readonly_input_context() const
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
void c_game_manager::sanitize_tile_offset(s_tile_position* offset)
{
	assert(offset != nullptr);

	offset->x = (offset->x < -TILE_MOVE_THRESHOLD) ? -TILE_MOVE_THRESHOLD : offset->x;
	offset->x = (offset->x > TILE_MOVE_THRESHOLD) ? TILE_MOVE_THRESHOLD : offset->x;
	offset->y = (offset->y < -TILE_MOVE_THRESHOLD) ? -TILE_MOVE_THRESHOLD : offset->y;
	offset->y = (offset->y > TILE_MOVE_THRESHOLD) ? TILE_MOVE_THRESHOLD : offset->y;
}

// Migrates the tile offset across tile boundary lines
void c_game_manager::migrate_tile_offset(
	s_tile_position* offset, 
	const s_tilemap_position& old_pos, 
	const s_tilemap_position& new_pos)
{
	assert(offset != nullptr);

	// $NOTE: MOST of the time speed will be slow enough such that we always catch this exactly at
	// offset 0.05f, but we cover the the additional case as well for things like teleportation or
	// slow operation.

	const double current_x = old_pos.x + offset->x;
	const double current_y = old_pos.y + offset->y;

	const double x_diff = current_x - new_pos.x;
	const double y_diff = current_y - new_pos.y;

	offset->x = x_diff;
	offset->y = y_diff;
}

void c_game_manager::sanitize_object_position(c_clobject* obj, const s_tilemap_position& pos)
{
	int max_tiles_x = m_current_tilemap->max_tiles_x;
	int max_tiles_y = m_current_tilemap->max_tiles_y;

	s_tile_position offset = obj->get_relative_tile_offset();

	if (offset_exceeded_tile_threshold(offset))
	{
		// $TODO: Handle case where speed is fast enough that we actually cross multiple tile borders in one frame???
		// i.e. teleportation support? How do we resolve collisions? Remain at the original spot or find adjacent free spot?

		int new_x = round_to_nearest_tile(pos.x + offset.x);
		int new_y = round_to_nearest_tile(pos.y + offset.y);

		const bool within_tilemap_bounds =
			(new_x >= 0)
			&& new_x < max_tiles_x
			&& new_y >= 0
			&& new_y < max_tiles_y;

		const bool new_position_empty =
			within_tilemap_bounds 
			&& m_current_tilemap->m_tiles[new_x][new_y] == nullptr;

		// Check if the new position is within bounds
		if (within_tilemap_bounds && new_position_empty)
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

int c_game_manager::round_to_nearest_tile(double value) const
{
	bool negative = value < 0;

	value = std::abs(value);
	int value_cieling = std::ceil(value);

	int rounded_value = ((value_cieling - value) < TILE_MOVE_THRESHOLD) 
		? value_cieling
		: std::floor(value);

	return negative ? -rounded_value : rounded_value;
}

void c_game_manager::draw_object(const c_clobject& obj, const s_tilemap_position& pos)
{
	// Draw object using game renderer
	assert(m_game_renderer != nullptr);
	m_game_renderer->draw_object(obj, pos);
}

void c_game_manager::draw_objects()
{
	assert(m_current_tilemap != nullptr);
	int max_tiles_x = m_current_tilemap->max_tiles_x;
	int max_tiles_y = m_current_tilemap->max_tiles_y;

	for (int x = 0; x < max_tiles_x; ++x)
	{
		for (int y = 0; y < max_tiles_y; ++y)
		{
			c_clobject* obj = m_current_tilemap->m_tiles[x][y];
			if (obj != nullptr)
			{
				draw_object(*obj, { x, y });
			}
		}
	}
}

// Used for debugging purposes to visualize tile boundaries
void c_game_manager::draw_tiles()
{
	assert(m_current_tilemap != nullptr);
	int max_tiles_x = m_current_tilemap->max_tiles_x;
	int max_tiles_y = m_current_tilemap->max_tiles_y;

	for (int x = 0; x < max_tiles_x; ++x)
	{
		for (int y = 0; y < max_tiles_y; ++y)
		{
			// Draw empty tile for debugging purposes
			assert(m_game_renderer != nullptr);
			m_game_renderer->draw_tile({ x, y });
		}
	}
}
