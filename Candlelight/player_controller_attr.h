#pragma once

// includes
#include "clattribute.h"

class c_player_controller_attr : public c_clattribute
{
public:
	c_player_controller_attr(c_clobject* owning_object)
		: c_clattribute(owning_object) {}

	void update() override;

	inline void set_move_speed(int move_speed) { m_move_speed = move_speed; }
private:
	void migrate_tiles();

	// Speed at which the player can move, in tiles per second
	int m_move_speed = 0;
};