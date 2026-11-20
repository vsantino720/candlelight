#pragma once

// includes
#include "clattribute.h"

class c_camera_follow_attr : public c_clattribute
{
public:
	c_camera_follow_attr(c_clobject* owning_object)
		: c_clattribute(owning_object) {}

	void update() override;
};