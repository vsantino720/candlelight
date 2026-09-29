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

struct s_event_mapping
{
	// Custom name to watch for the event name
	std::string event_name;

	// SDL type of the event we are looking for
	SDL_EventType event_type;
};

struct s_watched_event_entry
{
	SDL_EventType event_type;
	bool fired;
};

class c_watched_events_cache
{
public:
	c_watched_events_cache(std::vector<s_event_mapping>& event_mappings);
	void clear_cache();
	void log_event(const SDL_Event& event_type);
	bool event_was_fired(const SDL_EventType event_type) const;
private:
	// List of events triggered within last update
	// $TODO: Sort this list for accelerated searching??
	std::vector<s_watched_event_entry> m_watched_events_cache;
};

// Candlelight input manager class. This class is responsible for managing input events.
class c_input_manager
{
public:
	c_input_manager(
		std::vector<s_input_mapping>& input_mappings,
		std::vector<s_event_mapping>& event_mappings);

	bool input_is_pressed(const std::string& input_name) const;
	bool event_was_fired(const std::string& event_name) const;
	void update();

	inline void set_not_updated() { m_is_updated = false; }

private:
	// Sorted list of input mappings
	const std::vector<s_input_mapping> m_input_mappings;

	// Unsorted event mappings $TODO: Sort this list as well??
	const std::vector<s_event_mapping> m_event_mappings;

	// List of fired events from the last update
	c_watched_events_cache m_watched_events;

	bool m_is_updated = false;
};