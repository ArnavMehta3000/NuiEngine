#include "Component.h"

namespace Nui::ECS
{
	Component::Id Internal::s_componentCounter = 0;

	namespace  // Anonymous namespace
	{
		static Component::Id s_idCounter = 0;
	}  // Anonymous namespace

	Component::Component()
		: m_id(s_idCounter++)
	{
	}

	Component::Component(const Component& other)
	{
		this->m_id = s_idCounter++;
		this->m_entity = other.m_entity;
	}

	Component& Component::operator=(const Component& other) noexcept
	{
		this->m_id = s_idCounter;
		this->m_entity = other.m_entity;
		return *this;
	}


}