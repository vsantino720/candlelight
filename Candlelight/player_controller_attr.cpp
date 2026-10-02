// includes
#include "clobject.h"
#include "game_manager.h"
#include "input_manager.h"
#include "player_controller_attr.h"
#include "tilemap.h"

void c_player_controller_attr::update()
{
	const double delta_time = c_game_manager::get_instance().get_delta_time();

	// Update the player's target position based on input
	const c_input_manager& input_context = 
		c_game_manager::get_instance().get_readonly_input_context();

	s_tile_position current_position = m_owner->get_relative_tile_offset();

	// Handle the case where the player is not moving.
	// In our tile game, we only allow the player to 
	// move when they are not already moving. This prevents 
	// the player from changing direction mid-move.

	const bool is_moving = !current_position.is_clear();
	const double pos_diff = m_move_speed * delta_time;
		
	if (input_context.input_is_pressed("move_up"))
	{
		current_position.y -= pos_diff;
	}
	
	if (input_context.input_is_pressed("move_down"))
	{
		current_position.y += pos_diff;
	}
	
	if (input_context.input_is_pressed("move_left"))
	{
		current_position.x -= pos_diff;
	}
	
	if (input_context.input_is_pressed("move_right"))
	{
		current_position.x += pos_diff;
	}

	m_owner->set_relative_tile_offset(current_position);
}