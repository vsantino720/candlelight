#pragma once

#include <vector>
#include "clobject.h"

// Struct representing a position within a tile, in tile units
// We use floats here to allow for smooth movement between tiles, rather than snapping to tile positions.
struct s_tile_position
{
	float x;
	float y;

	inline bool is_clear() const { return x == 0.0f && y == 0.0f; }
	void clear();
};

// Struct representing a position on the tilemap
struct s_tilemap_position
{
	int x;
	int y;
};

// Struct representing a tilemap in the Candlelight engine.
struct s_tilemap
{
	s_tilemap(int x, int y) : max_tiles_x(x), max_tiles_y(y) {}
	
	// Dimensions of the tilemap in tiles. This is not the same as the
	// screen dimensions, which are in pixels.
	int max_tiles_x;
	int max_tiles_y;

	// 2D vector of tiles representing the tilemap
	std::vector<std::vector<c_clobject*>> m_tiles;
};