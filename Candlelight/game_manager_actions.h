#pragma once

// forward declarations
typedef t_tile_position t_tile_position;

enum e_game_manager_action
{
	NONE = 0,
	SET_VIEWPOINT
};

union u_game_manager_action
{
	e_game_manager_action action;
	s_game_manager_set_viewpoint_action set_viewpoint_action;
};

struct s_game_manager_set_viewpoint_action
{
	t_tile_position new_viewpoint;
};