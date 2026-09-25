#pragma once

#include <vector>
#include "clattribute.h"

// This is a class that represents a generic object in the Candlelight engine.
class c_clobject
{
private:
	// List of candlelight attributes attached to this clobject. Stored inline in the vector.
	std::vector<c_clattribute> m_attributes;
};