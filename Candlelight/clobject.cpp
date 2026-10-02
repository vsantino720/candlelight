// includes
#include "clobject.h"
#include "clattribute.h"

void c_clobject::update()
{
	for (const std::unique_ptr<c_clattribute>& attribute : m_attributes)
	{
		attribute->update();
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