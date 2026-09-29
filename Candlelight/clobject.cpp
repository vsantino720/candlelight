#include "clobject.h"

void c_clobject::update(float deltaTime, const c_game_context& game_context)
{
	for (c_clattribute& attribute : m_attributes)
	{
		attribute.update(deltaTime, game_context);
	}
}

void c_clobject::set_relative_tile_offset(const s_tile_position& pos)
{
	m_relative_tile_offset = pos;
}

const s_tile_position& c_clobject::get_relative_tile_offset() const
{
	return m_relative_tile_offset;
}

void c_clobject::clear_relative_tile_offset()
{
	m_relative_tile_offset = { 0, 0 };
}