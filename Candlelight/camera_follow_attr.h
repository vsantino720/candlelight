#pragma once

// includes
#include "clattribute.h"
#include "clobject.h"

// forward declaration
struct s_clobject_id;

class c_camera_follow_attr : public c_clattribute
{
public:
	c_camera_follow_attr(const s_clobject_id owning_object_id)
		: c_clattribute(owning_object_id) {}

	void update(c_clobject* owning_object) override;
};