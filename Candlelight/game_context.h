#pragma once

#include "input_manager.h"

// Candlelight game context class. This class is responsible for holding readonly game state.
class c_game_context
{
public:
	const c_input_manager& get_readonly_input_context() const;
	c_input_manager& get_input_context() const;
	void set_input_context(c_input_manager* input_context);

private:
	c_input_manager* m_input_context;
};