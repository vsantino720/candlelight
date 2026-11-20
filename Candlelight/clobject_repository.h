#pragma once

#include <unordered_map>
#include <stack>
#include "clobject.h"

// forward declarations
typedef int t_clobject_id;
typedef int t_clobject_pool_index;

constexpr int MAX_CLOBJECTS = 100; // Maximum number of c_clobject instances that can be created in the repository.

struct s_clobject_lookup_entry
{
	t_clobject_id id;
	t_clobject_pool_index index;

	bool operator<(const s_clobject_lookup_entry& other) const
	{
		return id < other.id;
	}

	bool operator==(const s_clobject_lookup_entry& other) const
	{
		return id == other.id;
	}

	bool operator<(const t_clobject_id& other_id) const
	{
		return id < other_id;
	}

	bool operator==(const t_clobject_id& other_id) const
	{
		return id == other_id;
	}
};

class c_clobject_lookup_table
{
public:
	void add_object(const t_clobject_id id, const t_clobject_pool_index index);
	void remove_object(const t_clobject_id id);
	const t_clobject_pool_index try_to_get_pool_index_for_id(const t_clobject_id id) const;
	const t_clobject_pool_index get_pool_index_for_id(const t_clobject_id id) const;
private:
	std::vector<s_clobject_lookup_entry> m_sorted_entries;
};

// Class responsible for creating and managing c_clobject instances. 
// This class is responsible for creating new c_clobject instances, 
// assigning them unique IDs, and storing them in a repository for 
// later retrieval. It also provides methods for retrieving c_clobject 
// instances by their unique ID.
class c_clobject_repository
{
public:
	c_clobject_repository();
	~c_clobject_repository() = default;

	c_clobject* try_to_create_object();
	c_clobject* get_object(t_clobject_id id);
	c_clobject* try_to_get_object(t_clobject_id id);
	void free_object(t_clobject_id id);

private:
	// Sorted list of clobject indices
	c_clobject_lookup_table m_object_lookup_table;

	// Repository of clobjects
	c_clobject m_objects_pool[MAX_CLOBJECTS];

	// Stack of free indices in the objects pool
	std::stack<t_clobject_pool_index> m_free_indices;

	t_clobject_id m_next_id = 1; // Next unique ID to assign to a new c_clobject instance.
};