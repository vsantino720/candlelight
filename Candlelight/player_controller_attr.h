#pragma once

// includes
#include "clattribute.h"
#include "clobject.h"

// forward declaration
struct s_clobject_id;

class c_player_controller_attr : public c_clattribute
{
public:
	c_player_controller_attr(const s_clobject_id owning_object_id)
		: c_clattribute(owning_object_id) {}

	void update(c_clobject* owning_object) override;

	inline void set_move_speed(int move_speed) { m_move_speed = move_speed; }
private:
	// Speed at which the player can move, in tiles per second
	int m_move_speed = 0;
};