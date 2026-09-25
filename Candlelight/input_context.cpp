#include "input_context.h"
#include <algorithm>
#include <assert.h>

// public methods

c_input_context::c_input_context(std::vector<s_input_mapping> input_mappings)
	: m_input_mappings(std::move(input_mappings))
{
	// Sort the input mappings by input_name for efficient searching
	std::sort(m_input_mappings.begin(), m_input_mappings.end());

	// TODO: Duplicate checking
};

void c_input_context::update()
{
	SDL_PumpEvents();
	m_is_updated = true;
}

bool c_input_context::is_input_pressed(const std::string& input_name) const
{
	assert(m_is_updated);

	bool is_pressed = false;
	int numkeys;

	// Use binary search to find the input mapping
	s_input_mapping empty_mapping = { input_name, SDL_SCANCODE_UNKNOWN };
	auto it = std::lower_bound(m_input_mappings.begin(), m_input_mappings.end(), empty_mapping);

	if (it != m_input_mappings.end() && it->input_name == input_name)
	{
		const bool* key_state = SDL_GetKeyboardState(&numkeys);
		is_pressed = (it->scancode < numkeys) && key_state[it->scancode];
	}

	return is_pressed;
}