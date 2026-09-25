#pragma once

#include <vector>
#include "clobject.h"

// Position in tiles-space, not screen-space (pixels)
struct TilePosition
{
	int x;
	int y;
};

// Class representing a tilemap in the Candlelight engine.
class c_tilemap
{
public:
	c_tilemap(int x, int y) : m_max_tiles_x(x), m_max_tiles_y(y) {}
private:
	// Dimensions of the tilemap in tiles. This is not the same as the
	// screen dimensions, which are in pixels.
	int m_max_tiles_x;
	int m_max_tiles_y;

	// 2D vector of clobjects representing the clobjects in the tilemap
	std::vector<std::vector<c_clobject>> m_clobjects;
};