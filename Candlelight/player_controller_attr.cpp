// includes
#include "clobject.h"
#include "game_manager.h"
#include "input_manager.h"
#include "player_controller_attr.h"
#include "vector.h"

void c_player_controller_attr::update(c_clobject* owning_object)
{
	const double delta_time = c_game_manager::get_instance().get_delta_time();

	// Update the player's target position based on input
	const c_input_manager& input_manager = 
		c_game_manager::get_instance().get_readonly_input_manager();

	t_tile_position current_position = owning_object->get_relative_tile_offset();

	s_vector<float> direction = { 0, 0 };

	int current_speed = m_move_speed;
		
	if (input_manager.input_is_pressed("move_up"))
	{
		direction.y -= 1;
	}
	
	if (input_manager.input_is_pressed("move_down"))
	{
		direction.y += 1;
	}
	
	if (input_manager.input_is_pressed("move_left"))
	{
		direction.x -= 1;
	}
	
	if (input_manager.input_is_pressed("move_right"))
	{
		direction.x += 1;
	}

	if (input_manager.input_is_pressed("speed_boost"))
	{
		current_speed *= m_speed_boost_scalar;
	}

	direction.normalize();

	current_position.x += direction.x * current_speed * delta_time;
	current_position.y += direction.y * current_speed * delta_time;

	owning_object->set_relative_tile_offset(current_position);
}