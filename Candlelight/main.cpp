// includes
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>

#include "clobject.h"
#include "input_manager.h"
#include "game_renderer.h"
#include "game_manager.h"
#include "tilemap.h"
#include "player_controller_attr.h"

int main(int argc, char* argv[]) 
{
	std::vector<s_input_mapping> input_mappings = {
		{ "move_up", SDL_SCANCODE_W },
		{ "move_down", SDL_SCANCODE_S },
		{ "move_left", SDL_SCANCODE_A },
		{ "move_right", SDL_SCANCODE_D }
	};

	std::vector<s_event_mapping> event_mappings = {
		{ "quit_game", SDL_EVENT_QUIT },
	};

	c_input_manager input_manager(input_mappings, event_mappings);

	c_clobject player_object;
	{
		c_player_controller_attr* player_controller_attr = 
			player_object.new_attribute<c_player_controller_attr>();
		player_controller_attr->set_move_speed(2); // 2 tiles per second
	}

	s_tilemap initial_tilemap(8, 6); // 8x6 tilemap
	initial_tilemap.m_tiles[4][3] = &player_object; // Place player in the center of the tilemap
	
	c_game_renderer game_renderer("Candlelight", TILE_SIZE * 8, TILE_SIZE * 6);

	c_game_manager* game_manager = 
		c_game_manager::try_to_initialize_game(
			&game_renderer, 
			&input_manager, 
			&initial_tilemap);

	if (game_manager == nullptr)
	{
		std::cerr << "Failed to initialize game manager." << std::endl;
		return 1;
	}

	const bool result = game_manager->run_game();

    return result ? 0 : 1;
}