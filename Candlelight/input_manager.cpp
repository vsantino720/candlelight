// includes
#include <algorithm>
#include <assert.h>
#include "input_manager.h"

// public methods

// c_watched_events_cache

c_watched_events_cache::c_watched_events_cache(
	const std::vector<s_event_mapping>& event_mappings)
{
	// Allocate all required memory at construction to provide predictable memory usage
	for (const s_event_mapping& event_mapping : event_mappings)
	{
		m_watched_events_cache.push_back({ event_mapping.event_type, false });
	}
}

void c_watched_events_cache::clear_cache()
{
	for (s_watched_event_entry& event_entry : m_watched_events_cache)
	{
		event_entry.fired = false;
	}
}

void c_watched_events_cache::log_event(const SDL_Event& event)
{
	for (s_watched_event_entry& event_entry : m_watched_events_cache)
	{
		if (event_entry.event_type == event.type)
		{
			event_entry.fired = true;
		}
	}
}

bool c_watched_events_cache::event_was_fired(const SDL_EventType event_type) const
{
	bool was_fired = false;

	for (const s_watched_event_entry& event_entry : m_watched_events_cache)
	{
		if (event_entry.event_type == event_type)
		{
			was_fired = event_entry.fired;
			break;
		}
	}

	return was_fired;
}

// c_input_manager

c_input_manager::c_input_manager(
	const std::vector<s_input_mapping>& input_mappings,
	const std::vector<s_event_mapping>& event_mappings)
		: m_input_mappings(input_mappings),
		m_event_mappings(event_mappings),
		m_watched_events(event_mappings)
{
	// Sort the input mappings by input_name for efficient searching
	std::sort(m_input_mappings.begin(), m_input_mappings.end());

	// TODO: Duplicate checking
};

void c_input_manager::update()
{
	m_watched_events.clear_cache();
	SDL_Event event;

	// SDL_PollEvent implicitly calls SDL_PumpEvents
	while (SDL_PollEvent(&event))
	{
		m_watched_events.log_event(event);
	}

	m_is_updated = true;
}

bool c_input_manager::input_is_pressed(const std::string& input_name) const
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

bool c_input_manager::event_was_fired(const std::string& event_name) const
{
	SDL_EventType event_type = SDL_EVENT_FIRST;

	for (s_event_mapping event_mapping : m_event_mappings)
	{
		if (event_mapping.event_name == event_name)
		{
			event_type = event_mapping.event_type;
			break;
		}
	}

	return (event_type != SDL_EVENT_FIRST) 
		&& m_watched_events.event_was_fired(event_type);
}