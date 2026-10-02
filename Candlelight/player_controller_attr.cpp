// includes
#include "clobject.h"
#include "game_manager.h"
#include "input_manager.h"
#include "player_controller_attr.h"
#include "vector.h"

void c_player_controller_attr::update()
{
	const double delta_time = c_game_manager::get_instance().get_delta_time();

	// Update the player's target position based on input
	const c_input_manager& input_context = 
		c_game_manager::get_instance().get_readonly_input_context();

	t_tile_position current_position = m_owner->get_relative_tile_offset();

	s_vector<float> direction = { 0, 0 };
		
	if (input_context.input_is_pressed("move_up"))
	{
		direction.y -= 1;
	}
	
	if (input_context.input_is_pressed("move_down"))
	{
		direction.y += 1;
	}
	
	if (input_context.input_is_pressed("move_left"))
	{
		direction.x -= 1;
	}
	
	if (input_context.input_is_pressed("move_right"))
	{
		direction.x += 1;
	}

	if (!direction.is_clear())
	{
		double magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		direction.x /= magnitude;
		direction.y /= magnitude;
	}

	current_position.x += direction.x * m_move_speed * delta_time;
	current_position.y += direction.y * m_move_speed * delta_time;

	m_owner->set_relative_tile_offset(current_position);
}