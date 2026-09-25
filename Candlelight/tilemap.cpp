#include "tilemap.h"

// public functions
c_tilemap::c_tilemap(int x, int y) : m_max_tiles_x(x), m_max_tiles_y(y)
{
	// Initialize the 2D vector of clobjects to the size of the tilemap
	m_clobjects.resize(m_max_tiles_x);
	for (int i = 0; i < m_max_tiles_x; i++)
	{
		m_clobjects[i].resize(m_max_tiles_y);
	}
}