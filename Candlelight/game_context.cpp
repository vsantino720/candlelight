#include "game_context.h"
#include <assert.h>

const c_input_context* c_game_context::get_readonly_input_context() const
{
	return get_input_context();
}

c_input_context* c_game_context::get_input_context() const
{
	assert(m_input_context != nullptr);
	return m_input_context;
}

void c_game_context::set_input_context(c_input_context* input_context)
{
	m_input_context = input_context;
}