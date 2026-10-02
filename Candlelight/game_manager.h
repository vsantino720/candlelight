#pragma once

// includes
#include "tilemap.h"
#include "vector.h"

// forward declarations
class c_clobject;
class c_game_renderer;
class c_input_manager;

// Distance in tiles that an object must move before it is considered to have moved to a new tile.
constexpr float TILE_MOVE_THRESHOLD = 0.5f;
constexpr const char* QUIT_GAME_EVENT_NAME = "quit_game";

// Game manager singleton handles all update loop logic on downstream objects, active tilemap,
// and tilemap transitions.
class c_game_manager
{
public:
	// We split these operations in two to protect that only the creator
	// of the singleton can modify it. All other access is read-only.
	static c_game_manager* try_to_initialize_game(
		c_game_renderer* game_renderer,
		c_input_manager* input_context,
		s_tilemap* initial_tilemap);
	static const c_game_manager& get_instance();

	bool run_game();

	// Getters and setters
	c_input_manager& get_input_context() const;
	void set_input_context(c_input_manager* input_context);
	void set_current_tilemap(s_tilemap* tilemap);
	void set_game_renderer(c_game_renderer* game_renderer);

	// Read-only access to the input context for external systems that need to query input state.
	const c_input_manager& get_readonly_input_context() const;
	inline const double get_delta_time() const { return m_delta_time; }

private:
	c_game_manager() = default; // Private constructor
	static c_game_manager* m_instance; // Static instance pointer

	// Update the game state, including all objects in the current tilemap.
	void update_objects();

	// Position
	void sanitize_tile_offset(t_tile_position* offset);
	void migrate_tile_offset(
		t_tile_position* offset, 
		const t_tilemap_position& old_pos,
		const t_tilemap_position& new_pos);
	void sanitize_object_position(c_clobject* obj, const t_tilemap_position& pos);
	inline bool offset_exceeded_tile_threshold(const t_tile_position offset) const
	{
		return (offset.x < -TILE_MOVE_THRESHOLD)
			|| (offset.x > TILE_MOVE_THRESHOLD)
			|| (offset.y < -TILE_MOVE_THRESHOLD)
			|| (offset.y > TILE_MOVE_THRESHOLD);
	};

	int round_to_nearest_tile(double value) const;

	// Drawing
	void draw_object(const c_clobject& obj, const t_tilemap_position& pos);
	void draw_objects();
	void draw_tiles();

	// member variables
	// Current tilemap to render and update.
	s_tilemap* m_current_tilemap = nullptr;
	c_game_renderer* m_game_renderer = nullptr;
	c_input_manager* m_input_context = nullptr;
	double m_delta_time = 0.0;
};