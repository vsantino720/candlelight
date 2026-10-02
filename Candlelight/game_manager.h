#pragma once

// Distance in tiles that an object must move before it is considered to have moved to a new tile.
#define TILE_MOVE_THRESHOLD 0.5f

#include "game_renderer.h"
#include "input_manager.h"
#include "tilemap.h"

// Game manager singleton handles all update loop logic on downstream objects, active tilemap,
// and tilemap transitions.
class c_game_manager
{
public:
	// We split these operations in two to protect that only the creator
	// of the singleton can modify it. All other access is read-only.
	static c_game_manager* try_to_create_instance();
	static const c_game_manager& get_instance();

	// Update the game state, including all objects in the current tilemap.
	void update(float deltaTime);

	// Input Manager
	const c_input_manager& get_readonly_input_context() const;
	c_input_manager& get_input_context() const;
	void set_input_context(c_input_manager* input_context);

	void set_current_tilemap(s_tilemap* tilemap);

private:
	c_game_manager() = default; // Private constructor
	static c_game_manager* instance; // Static instance pointer

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
	c_input_manager* m_input_context;
};