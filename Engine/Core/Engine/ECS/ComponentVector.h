#pragma once
#include "Core/Engine/ECS/Component.h"

namespace Nui::ECS
{ 
	template <typename C = Component>
	class ComponentVector final : public std::vector<std::shared_ptr<Component>>
	{
		using Pointer = std::shared_ptr<C>;
		using Container = std::vector<std::shared_ptr<Component>>;
	public:
		Pointer Get(size_t n) { return std::static_pointer_cast<C>(at(n)); }
		Pointer First() { return Get(0); }

		explicit operator bool() const { return !empty(); }
		Pointer operator*() { return First(); }

		template <typename T = Component>
		ComponentVector<C>& operator=(const ComponentVector<T>& other)
		{
			using vect = std::vector<std::shared_ptr<Component>>;
			*static_cast<vect*>(this) = *static_cast<const vect*>(&other);
			return *this;
		}

		template <typename T = Component>
		ComponentVector<C>& operator=(ComponentVector<T>&& other)
		{
			*static_cast<Container*>(this) = std::move(*static_cast<const Container*>(&other));
			return *this;
		}
	};
}