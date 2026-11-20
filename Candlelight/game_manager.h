#pragma once

// includes
#include "tilemap.h"
#include "vector.h"

// forward declarations
class c_clobject;
class c_game_renderer;
class c_input_manager;
class c_clobject_repository;
struct s_clobject_id;

// Distance in tiles that an object must move before it is considered to have moved to a new tile.
constexpr float TILE_MOVE_THRESHOLD = 1.0f;
constexpr const char* QUIT_GAME_EVENT_NAME = "quit_game";

struct s_tilemap_object_id	
{
	s_clobject_id object_id;
	t_tilemap_position position;
};

struct s_tilemap_object
{
	c_clobject* object;
	t_tilemap_position position;

	inline bool is_valid() const 
	{
		return object != nullptr;
	}
};

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
		c_clobject_repository* clobject_repository,
		s_tilemap* initial_tilemap);
	static const c_game_manager& get_instance();

	bool run_game();

	// Getters and setters
	c_input_manager& get_input_context() const;
	const c_clobject* try_to_get_readonly_object(const s_clobject_id id) const;
	const c_clobject& get_readonly_object(const s_clobject_id id) const;
	const t_tilemap_position try_to_get_object_position(const s_clobject_id id) const;
	const t_tilemap_position get_object_position(const s_clobject_id id) const;
	void set_input_context(c_input_manager* input_context);
	void set_current_tilemap(s_tilemap* tilemap);
	void set_game_renderer(c_game_renderer* game_renderer);
	void set_clobject_repository(c_clobject_repository* clobject_repository);

	// Read-only access to the input manager for external systems that need to query input state.
	const c_input_manager& get_readonly_input_manager() const;
	inline const double get_delta_time() const { return m_delta_time; }

private:
	c_game_manager() = default; // Private constructor
	static c_game_manager* m_instance; // Static instance pointer

	// Update the game state, including all objects in the current tilemap.
	void update_objects();
	void process_pending_actions_for_object(s_tilemap_object tilemap_obj);

	// Position
	void sanitize_tile_offset(t_tile_position* offset);
	void migrate_tile_offset(
		t_tile_position* offset, 
		const t_tilemap_position& old_pos,
		const t_tilemap_position& new_pos);
	void sanitize_object_position(s_tilemap_object tilemap_obj);

	inline bool offset_exceeded_tile_threshold(const t_tile_position offset) const
	{
		return (offset.x < 0)
			|| (offset.x > TILE_MOVE_THRESHOLD)
			|| (offset.y < 0)
			|| (offset.y > TILE_MOVE_THRESHOLD);
	};

	// member variables
	// Current tilemap to render and update.
	s_tilemap* m_current_tilemap = nullptr;
	c_game_renderer* m_game_renderer = nullptr;
	c_input_manager* m_input_context = nullptr;
	c_clobject_repository* m_clobject_repository = nullptr;

	// Time since last frame
	double m_delta_time = 0.0;

	// Viewpoint position in tilemap coordinates. This is the center of the screen in tilemap coordinates.
	t_tile_position m_viewpoint_position = { 0.0f, 0.0f };
};