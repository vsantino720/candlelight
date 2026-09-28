#pragma once

// Distance in tiles that an object must move before it is considered to have moved to a new tile.
#define TILE_MOVE_THRESHOLD 0.5f

#include "game_context.h"
#include "tilemap.h"

constexpr int TILE_SIZE = 32; // pixels

class c_game_manager
{
public:
	c_game_manager(s_tilemap* tilemap)
		: m_current_tilemap(tilemap) {};

	void update(float deltaTime);
private:
	void sanitize_tile_offset(s_tile_position* offset);
	void migrate_tile_offset(
		s_tile_position* offset, 
		const s_tilemap_position& old_pos,
		const s_tilemap_position& new_pos);
	void sanitize_object_position(c_clobject* obj, const s_tilemap_position& pos);

	inline bool position_moved_tiles(const s_tile_position offset) const
	{
		return (offset.x < -TILE_MOVE_THRESHOLD)
			|| (offset.x > TILE_MOVE_THRESHOLD)
			|| (offset.y < -TILE_MOVE_THRESHOLD)
			|| (offset.y > TILE_MOVE_THRESHOLD);
	}

	// Current tilemap to render and update.
	s_tilemap* m_current_tilemap;
	c_game_context m_game_context;
};