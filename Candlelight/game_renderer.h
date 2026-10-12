#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "tilemap.h"

// macros
#define DEBUG_ENABLED

// forward declarations
class c_clobject;

constexpr const char* WINDOW_RESIZE_EVENT_NAME = "window_resize";

constexpr const float OBJECT_SCALAR = 0.5f; // Scales dimensions of an object in tile units

// typedefs
// Screen position is in tile units, but are 0-indexed like the window coordinates
typedef s_vector<float> t_screen_tile_position;
typedef s_vector<float> t_screen_position;
typedef s_vector<float> t_screen_dimensions;

// Renders game objects
class c_game_renderer
{
public:
	c_game_renderer(
		const char* window_name,
		int window_width,
		int window_height) :
			m_window_width(window_width),
			m_window_height(window_height),
			m_window_name(window_name) {};

	bool initialize_window();
	void update();
	void draw_tilemap_from_viewpoint(const s_tilemap& tilemap, const t_tile_position& pos);
	void render();
	~c_game_renderer();

private:
	inline t_screen_dimensions get_tile_size() const
	{ 
		return 
		{ 
			(float)m_window_width / (float)m_view_width, 
			(float)m_window_height / (float)m_view_height 
		};
	};

	inline bool texture_is_viewable(
		const t_screen_tile_position& pos,
		t_screen_dimensions texture_dimensions) const
	{
		return (pos.x + (texture_dimensions.x / 2.0f)) >= 0 
			&& (pos.x - (texture_dimensions.x / 2.0f)) < (m_view_width) 
			&& pos.y + (texture_dimensions.y / 2.0f) >= 0 
			&& pos.y - (texture_dimensions.y / 2.0f) < (m_view_height);
	};

	t_screen_tile_position tilemap_to_screen_tile_position(
		const t_tile_position& tilemap_pos,
		const t_tile_position& viewpoint_pos) const;

	t_screen_position screen_tile_to_screen_position(
		const t_screen_tile_position& screen_tile_pos) const;

	// Drawing
	void try_to_draw_object(const c_clobject& obj, const t_screen_tile_position& pos);
	void try_to_draw_tile(const t_screen_tile_position& screen_pos);
	void draw_background();

	SDL_Renderer* m_renderer = nullptr;
	SDL_Window* m_window = nullptr;
	int m_window_width = 1600;
	int m_window_height = 900;
	int m_view_width = 16; // in tiles
	int m_view_height = 9; // in tiles
	std::string m_window_name;
};