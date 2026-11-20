#include <algorithm>
#include <assert.h>
#include "clobject_repository.h"
#include "clattribute.h"

// c_clobject_lookup_table

// public methods

void c_clobject_lookup_table::add_object(const s_clobject_id id, const t_clobject_pool_index index)
{
	// Search the array for the sorted insertion point
	int insertion_point = std::lower_bound(m_sorted_entries.begin(), m_sorted_entries.end(), id) - m_sorted_entries.begin();
	m_sorted_entries.insert(m_sorted_entries.begin() + insertion_point, { id, index });
}

void c_clobject_lookup_table::remove_object(const s_clobject_id id)
{
	// Search the array for the object and remove it
	auto it = std::lower_bound(m_sorted_entries.begin(), m_sorted_entries.end(), id);
	if (it != m_sorted_entries.end() && it->id == id) 
	{
		m_sorted_entries.erase(it);
	}
}

const t_clobject_pool_index c_clobject_lookup_table::try_to_get_pool_index_for_id(const s_clobject_id id) const
{
	t_clobject_pool_index pool_index = -1;

	if (id != INVALID_ID)
	{
		auto it = std::lower_bound(m_sorted_entries.begin(), m_sorted_entries.end(), id);
		if (it != m_sorted_entries.end() && it->id == id)
		{
			pool_index = it->index;
		}
	}

	return pool_index;
}

const t_clobject_pool_index c_clobject_lookup_table::get_pool_index_for_id(const s_clobject_id id) const
{
	t_clobject_pool_index pool_index = try_to_get_pool_index_for_id(id);
	assert(pool_index != -1); // Ensure that the object was found
	return pool_index;
}

// c_clobject_repository

// public methods

c_clobject_repository::c_clobject_repository()
{
	m_free_indices.push(0); // Start with the first index available
}

c_clobject* c_clobject_repository::try_to_create_object()
{
	c_clobject* obj = nullptr;

	if (!m_free_indices.empty())
	{
		t_clobject_pool_index index = m_free_indices.top();
		m_free_indices.pop();

		obj = &m_objects_pool[index];
		obj->initialize(m_next_id++);

		// Sorted add
		m_object_lookup_table.add_object(obj->get_id(), index);

		// If the free indices stack is empty, we will push the next index onto the stack for future use
		// NOTE: If the free indices stack is empty, this means that we are adding to the tail of the objects pool
		if (m_free_indices.empty())
		{
			m_free_indices.push(index + 1);
		}
	}

	return obj;	
}

void c_clobject_repository::free_object(s_clobject_id id)
{
	c_clobject* obj = try_to_get_object(id);

	if (obj)
	{
		m_object_lookup_table.remove_object(id);
		obj->invalidate();
		m_free_indices.push(obj - m_objects_pool);
	}
}

c_clobject* c_clobject_repository::try_to_get_object(s_clobject_id id)
{
	c_clobject* obj = nullptr;

	t_clobject_pool_index index = m_object_lookup_table.try_to_get_pool_index_for_id(id);
	if (index != -1)
	{
		obj = &m_objects_pool[index];
	}

	return obj;
}

c_clobject* c_clobject_repository::get_object(s_clobject_id id)
{
	c_clobject* obj = try_to_get_object(id);
	assert(obj != nullptr); // Ensure that the object was found
	return obj;
}