#pragma once

#include <vector>
#include <string>
#include <SDL3/SDL.h>

struct s_input_mapping
{
	// The action to perform when the input is detected
	std::string input_name;
	// The SDL scancode for the input
	SDL_Scancode scancode;

	bool operator<(const s_input_mapping& other) const
	{
		return input_name < other.input_name;
	}
};

// Candlelight input context class. This class is responsible for managing input events.
class c_input_context
{
public:
	c_input_context(std::vector<s_input_mapping> input_mappings = {});

	bool is_input_pressed(const std::string& input_name) const;
	void update();
	void set_not_updated() { m_is_updated = false; }

private:
	// Sorted list of input mappings
	std::vector<s_input_mapping> m_input_mappings;
	bool m_is_updated = false;
};