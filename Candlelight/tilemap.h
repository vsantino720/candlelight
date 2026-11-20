#pragma once

#include <vector>
#include <cmath>

constexpr int INVALID_TILEMAP_POSITION = INT32_MAX;

// forward declarations
class c_clobject;

template <typename T>
struct s_vector;
typedef int t_clobject_id;

// typedefs
typedef s_vector<int> t_tilemap_position;
typedef s_vector<float> t_tile_position;

// Struct representing a tilemap in the Candlelight engine.
struct s_tilemap
{
	s_tilemap(int x, int y);
	
	// Dimensions of the tilemap in tiles. This is not the same as the
	// screen dimensions, which are in pixels.
	int max_tiles_x;
	int max_tiles_y;

	// $TODO: Does this need to be a vector?
	// 2D vector of tiles representing the tilemap
	std::vector<std::vector<t_clobject_id>> m_tiles;
};