#pragma once

#include "clattribute.h"
#include "tilemap.h"

class c_player_controller : public c_clattribute
{
public:
	c_player_controller(c_clobject& owning_object, int speed) 
		: c_clattribute(owning_object), m_speed(speed) {}

	void update(float deltaTime) override;

private:
	// Tiles per second
	int m_speed;
	// Position the player is moving towards, relative to the player's position
	TilePosition m_target_position;
};