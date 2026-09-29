#pragma once

#include <SDL3/SDL.h>
#include "tilemap.h"

#define TILE_SIZE 32 // pixels

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

	bool initilize_window();
	void draw_object(const c_clobject& obj, const s_tilemap_position& pos);
	void draw_background();
	void render();
	~c_game_renderer();
private:
	SDL_Renderer* m_renderer = nullptr;
	SDL_Window* m_window = nullptr;
	int m_window_width = 960;
	int m_window_height = 640;
	std::string m_window_name;
};