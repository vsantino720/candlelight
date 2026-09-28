#pragma once

#include <string>
#include "clobject.h"
#include "game_context.h"

/// Base candlelight attribute class. All attributes should inherit from this class.
/// Attributes provide modular behaviors to objects in the Candlelight engine. 
/// 
/// For example, an attribute could be a health component, a movement component, 
/// or a rendering component. By using attributes, we can easily add or remove behaviors 
/// from objects without modifying the core object class.
class c_clattribute
{
public:
	// Every clattribute must have a reference to the object that owns it. 
	// This allows the attribute to interact with its owning object.
	c_clattribute(c_clobject& owning_object) : m_owner(owning_object) {}

	// Called every frame to update the attribute
	virtual void update(float deltaTime, const c_game_context& game_context) = 0;
protected:
	// If true, only one instance of this attribute can be added to an object
	bool m_is_singleton = false;

	c_clobject& m_owner; // The object that owns this attribute
};