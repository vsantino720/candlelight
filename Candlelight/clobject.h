#pragma once

#include <vector>
#include "clattribute.h"
#include "tilemap.h"
#include "game_context.h"

// This is a class that represents a generic object in the Candlelight engine.
// Every object can have multiple attributes attached to it, which define its behavior and properties.
// Similarly, each object has a position in the tilemap, and a speed associated with moving the object.
class c_clobject
{
public:
	void update(float deltaTime, const c_game_context& game_context);
	void set_relative_tile_offset(const s_tile_position& pos);
	const s_tile_position& get_relative_tile_offset() const;
	void clear_relative_tile_offset();

private:
	// List of candlelight attributes attached to this clobject. Stored inline in the vector.
	std::vector<c_clattribute> m_attributes;

	// Distance from the current tile-center, in tile units.
	s_tile_position m_relative_tile_offset;
};