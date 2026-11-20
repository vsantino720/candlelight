// includes
#include "camera_follow_attr.h"
#include "clobject.h"
#include "game_manager_actions.h"

void c_camera_follow_attr::update()
{
	s_game_manager_action action;
	action.action_type = e_game_manager_action::SET_VIEWPOINT_FROM_OBJECT;
	action.action_data.set_viewpoint_from_object.offset_from_object = { 0, 0 };

	m_owner->queue_action(action);
}