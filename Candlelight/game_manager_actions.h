#pragma once

#include "tilemap.h"
#include "vector.h"

enum e_game_manager_action
{
	NONE = 0,
	SET_VIEWPOINT_FROM_OBJECT // Set the renderer viewpoint to the position of the object requesting the action
};

struct s_game_manager_set_viewpoint_from_object
{
	t_tile_position offset_from_object;
};

union u_game_manager_action_data
{
	s_game_manager_set_viewpoint_from_object set_viewpoint_from_object;
};

struct s_game_manager_action
{
	e_game_manager_action action_type;
	u_game_manager_action_data action_data;
};