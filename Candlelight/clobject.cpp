// includes
#include <assert.h>
#include "clobject.h"
#include "clobject_id.h"
#include "clattribute.h"

// c_clobject

// public methods

void c_clobject::initialize(const s_clobject_id id)
{
	m_id = id;
	center_relative_tile_offset();
}

void c_clobject::update()
{
	for (const std::unique_ptr<c_clattribute>& attribute : m_attributes)
	{
		attribute->update(this);
	}
}

void c_clobject::set_relative_tile_offset(const t_tile_position& pos)
{
	m_relative_tile_offset = pos;
}

const t_tile_position& c_clobject::get_relative_tile_offset() const
{
	return m_relative_tile_offset;
}

void c_clobject::center_relative_tile_offset()
{
	m_relative_tile_offset = { 0.5f, 0.5f };
}

void c_clobject::invalidate()
{
	m_id = INVALID_ID;
}

const s_game_manager_action* c_clobject::try_to_get_next_pending_action() const
{
	const s_game_manager_action* action = nullptr;

	if (!m_pending_actions.empty())
	{
		action = &m_pending_actions.front();
	}

	return action;
}

const s_game_manager_action& c_clobject::get_next_pending_action() const
{
	const s_game_manager_action* action = try_to_get_next_pending_action();
	assert(action != nullptr); // Ensure that there is a pending action to return
	return *action;
}

void c_clobject::pop_next_pending_action()
{
	if (!m_pending_actions.empty())
	{
		m_pending_actions.pop();
	}
}

bool c_clobject::has_pending_actions() const
{
	return !m_pending_actions.empty();
}

void c_clobject::queue_action(const s_game_manager_action& action)
{
	m_pending_actions.push(action);
}