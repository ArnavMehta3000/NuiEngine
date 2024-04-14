#pragma once
#include "Core/Common/CommonHeaders.h"

namespace Nui::ECS
{
	class Entity;

	class Component
	{
		friend class Entity;
	public:
		using Id = U64;
		using TypeId = U32;

	public:
		Component();
		Component(const Component& other);
		Component(Component&& other) = default;
		virtual ~Component() = default;


		Component& operator=(const Component& other) noexcept;
		Component& operator=(Component&&) noexcept = default;

		inline Id GetId() const { return m_id; }
		virtual bool IsUnique() { return true; }
		inline Entity* GetOwner() const { return m_entity; }
		inline bool IsDirty() const { return m_isDirty; }

	private:
		U8 m_isDirty;
		Id m_id;
		Entity* m_entity;
	};

	inline bool operator==(const Component& lhs, const Component& rhs)
	{
		return lhs.GetId() == rhs.GetId();
	}

	inline bool operator!=(const Component& lhs, const Component& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename C, template <typename> class smartPtr>
	inline C* ComponentCast(smartPtr<Component> ptr)
	{
		return static_cast<C*>(ptr.get());
	}

	namespace Internal
	{
		extern Component::Id s_nextId;

		template <typename T>
		struct TypeId 
		{
			static Component::Id Value()
			{
				static Component::Id typeId = s_nextId++;
				return typeId;
			}
		};
	}

	template <typename T>
	static inline Component::Id GetTypeId()
	{
		return Internal::TypeId<T>::Value();
	}
}