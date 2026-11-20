#pragma once

// includes
#include "clobject_id.h"

// forward decl
class c_clobject;

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
	c_clattribute(const s_clobject_id owning_object_id) : owning_object_id(owning_object_id) {}

	// Called every frame to update the attribute
	virtual void update(c_clobject* owning_object) {};
protected:
	// If true, only one instance of this attribute can be added to an object
	bool m_is_singleton = false;

	s_clobject_id owning_object_id = INVALID_ID;
};