#pragma once

#include <vector>
#include <memory>
#include "tilemap.h"
#include "vector.h"

// forward declarations
class c_clattribute;

// This is a class that represents a generic object in the Candlelight engine.
// Every object can have multiple attributes attached to it, which define its behavior and properties.
// Similarly, each object has a position in the tilemap, and a speed associated with moving the object.
class c_clobject
{
public:
	void update();
	void set_relative_tile_offset(const t_tile_position& pos);
	const t_tile_position& get_relative_tile_offset() const;
	void clear_relative_tile_offset();

	template <typename T>
	T* new_attribute()
	{
		m_attributes.emplace_back(std::make_unique<T>(this));
		return dynamic_cast<T*>(m_attributes.back().get());
	}

private:
	// List of candlelight attributes attached to this clobject.
	std::vector<std::unique_ptr<c_clattribute>> m_attributes;

	// Distance from the current tile-center, in tile units.
	t_tile_position m_relative_tile_offset = { 0, 0 };
};