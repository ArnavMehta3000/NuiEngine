#include "Component.h"

namespace Nui::ECS
{
	namespace Internal
	{
		Component::Id s_nextId = 0;
	}

	namespace  // Anonymous namespace
	{
		static Component::Id s_idCounter = 0;
	}  // Anonymous namespace

	Component::Component()
		: m_id(s_idCounter++)
		, m_entity(nullptr)
		, m_isDirty(true)
	{
	}

	Component::Component(const Component& other)
	{
		this->m_id = s_idCounter++;
		this->m_entity = other.m_entity;
		this->m_isDirty = other.m_isDirty;
	}

	Component& Component::operator=(const Component& other) noexcept
	{
		this->m_id = s_idCounter;
		this->m_entity = other.m_entity;
		this->m_isDirty = other.m_isDirty;
		return *this;
	}
}