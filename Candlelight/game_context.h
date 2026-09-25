#pragma once

#include "input_context.h"
// Candlelight game context class. This class is responsible for managing the game state and the main game loop.
class c_game_context
{
public:
	const c_input_context* get_readonly_input_context() const;
	c_input_context* get_input_context() const;
	void set_input_context(c_input_context* input_context);

private:
	c_input_context* m_input_context;
};