#pragma once

// Distance in tiles that an object must move before it is considered to have moved to a new tile.
#define TILE_MOVE_THRESHOLD 0.5f

#include "game_context.h"
#include "game_renderer.h"
#include "tilemap.h"

// Game manager handles all update loop logic on downstream objects, active tilemap,
// and tilemap transitions.
class c_game_manager
{
public:
	c_game_manager(s_tilemap* tilemap, c_game_renderer* game_renderer)
		: m_current_tilemap(tilemap), m_game_renderer(game_renderer) {};

	void update(float deltaTime);
private:
	// Position
	void sanitize_tile_offset(s_tile_position* offset);
	void migrate_tile_offset(
		s_tile_position* offset, 
		const s_tilemap_position& old_pos,
		const s_tilemap_position& new_pos);
	void sanitize_object_position(c_clobject* obj, const s_tilemap_position& pos);

	// Drawing
	void draw_object(const c_clobject& obj, const s_tilemap_position& pos);

	// inline
	inline bool offset_exceeded_tile_threshold(const s_tile_position offset) const
	{
		return (offset.x < -TILE_MOVE_THRESHOLD)
			|| (offset.x > TILE_MOVE_THRESHOLD)
			|| (offset.y < -TILE_MOVE_THRESHOLD)
			|| (offset.y > TILE_MOVE_THRESHOLD);
	}

	// member variables
	// Current tilemap to render and update.
	s_tilemap* m_current_tilemap;
	c_game_renderer* m_game_renderer;
	c_game_context m_game_context;
};