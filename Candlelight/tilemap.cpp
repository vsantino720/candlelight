#include "tilemap.h"

// public functions

// s_tile_position
void s_tile_position::clear()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

// s_tilemap
s_tilemap::s_tilemap(int x, int y) : m_max_tiles_x(x), m_max_tiles_y(y)
{
	// Initialize the 2D vector of tiles to the size of the tilemap
	m_tiles.resize(m_max_tiles_x);
	for (int i = 0; i < m_max_tiles_x; i++)
	{
		m_tiles[i].resize(m_max_tiles_y);
	}
}

