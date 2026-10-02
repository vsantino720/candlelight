#pragma once

#include "clattribute.h"
#include "tilemap.h"

class c_player_controller_attr : public c_clattribute
{
public:
	c_player_controller_attr(c_clobject& owning_object, int speed)
		: c_clattribute(owning_object), m_move_speed(speed) {}

	void update(float deltaTime) override;
private:
	// Speed at which the player can move, in tiles per second
	int m_move_speed;
};