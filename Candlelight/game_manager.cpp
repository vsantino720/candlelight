#include <assert.h>
#include "game_manager.h"

// public methods

void c_game_manager::update(float deltaTime)
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
					// Render object
					if (m_game_renderer != nullptr)
					{
						m_game_renderer->draw_object(*obj, { x, y });
					}

					// Run the update function for the object, which will in turn update its attributes
					obj->update(deltaTime, m_game_context);

					// Check if the object has moved to a new tile, and enforce collision and tilebounds
					sanitize_object_position(obj, { x, y });
				}
			}
		}
	}
}

// private methods

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

	int x_diff = (new_pos.x - old_pos.x);
	int y_diff = (new_pos.y - old_pos.y);

	// Since we are migrating across tile lines, we must reflect our offset to the
	// side of the boundary. i.e. (-0.6 -> +0.4), (+0.7 -> -0.3), (+1.8, -0.2)

	// $NOTE: MOST of the time speed will be slow enough such that we always catch this exactly at
	// offset 0.05f, but we cover the the additional case as well for things like teleportation or
	// slow operation.
	offset->x = (offset->x < -TILE_MOVE_THRESHOLD) ? (offset->x + x_diff) : offset->x;
	offset->x = (offset->x > TILE_MOVE_THRESHOLD) ? (x_diff - offset->x) : offset->x;
	offset->y = (offset->y < -TILE_MOVE_THRESHOLD) ? (offset->y + y_diff) : offset->y;
	offset->y = (offset->y > TILE_MOVE_THRESHOLD) ? (y_diff - offset->y) : offset->y;
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

		int new_x = pos.x + static_cast<int>(offset.x);
		int new_y = pos.y + static_cast<int>(offset.y);

		const bool within_tilemap_bounds =
			(new_x >= 0)
			&& new_x < max_tiles_x
			&& new_y >= 0
			&& new_y < max_tiles_y;

		const bool new_position_empty =
			m_current_tilemap->m_tiles[new_x][new_y] == nullptr;

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

void c_game_manager::draw_object(const c_clobject& obj, const s_tilemap_position& pos)
{

}