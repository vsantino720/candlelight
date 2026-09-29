#include "player_controller_attr.h"
#include <SDL3/SDL.h>

void c_player_controller_attr::update(float deltaTime, const c_game_context& game_context)
{
	// Update the player's target position based on input
	const c_input_manager& input_context = game_context.get_readonly_input_context();

	s_tile_position current_position = m_owner.get_relative_tile_offset();

	// Handle the case where the player is not moving.
	// In our tile game, we only allow the player to 
	// move when they are not already moving. This prevents 
	// the player from changing direction mid-move.
		
	if (current_position.y < 0.0f || input_context.input_is_pressed("move_up"))
	{
		current_position.y -= m_move_speed * deltaTime;
	}
	else if (current_position.y > 0.0f || input_context.input_is_pressed("move_down"))
	{
		current_position.y += m_move_speed * deltaTime;
	}
	else if (current_position.x < 0.0f || input_context.input_is_pressed("move_left"))
	{
		current_position.x -= m_move_speed * deltaTime;
	}
	else if (current_position.x > 0.0f || input_context.input_is_pressed("move_right"))
	{
		current_position.x += m_move_speed * deltaTime;
	}

	m_owner.set_relative_tile_offset(current_position);
}