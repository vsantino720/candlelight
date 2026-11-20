#pragma once

constexpr int INVALID_ID = 0;

struct s_clobject_id
{
	s_clobject_id(const int v = INVALID_ID) : value(v) {}

	int value;

	inline bool operator==(const s_clobject_id& other) const
	{
		return value == other.value;
	}

	inline bool operator!=(const s_clobject_id& other) const
	{
		return value != other.value;
	}

	inline bool operator<(const s_clobject_id& other) const
	{
		return value < other.value;
	}

	inline bool operator>(const s_clobject_id& other) const
	{
		return value > other.value;
	}

	// Postfix increment operator
	s_clobject_id operator++(int)
	{
		int old = value;
		value++;
		return old;
	}

	inline bool is_valid() const
	{
		return value != INVALID_ID;
	}
};