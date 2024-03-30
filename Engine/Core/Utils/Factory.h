#pragma once
#include "Core/Common/CommonHeaders.h"
#include <typeinfo>
#include <utility>

namespace Nui
{
	template <typename Base, typename... Args>
	class Factory
	{
		using CreatorFuncT = std::unique_ptr<Base>(*)(Args...);

	public:
		template <typename Type, class ... TArgs>
		static std::unique_ptr<Type> Create(TArgs&&... args)
		{
			const std::string name = typeid(Type).name();
			std::unique_ptr<Base> basePtr =Data().at(name)(std::forward<TArgs>(args)...);

			Type* derivedPtr = dynamic_cast<Type*>(basePtr.get());
			NUI_ASSERT(derivedPtr, "Failed to cast to derived class");
			basePtr.release();
			return std::unique_ptr<Type>(derivedPtr);
		}

		template <typename T> 
		struct Registrar : Base 
		{
			friend T;

			static bool RegisterType()
			{
				const std::string name = typeid(T).name();
				Factory::Data()[name] = [](Args... args) -> std::unique_ptr<Base> 
				{
					return std::make_unique<T>(std::forward<Args>(args)...);
				};

				return true;
			}

			static bool s_registered;

		private:
			Registrar() : Base(Key{}) { (void)s_registered; }
		};

	protected:
		class Key
		{
			Key() {}
			template <typename T>
			friend struct Registrar;
		};

	private:
		static std::unordered_map<std::string, CreatorFuncT>& Data()
		{
			static std::unordered_map<std::string, CreatorFuncT> data;
			return data;
		}
	};

	template <class Base, class... Args>
	template <class T>
	bool Factory<Base, Args...>::Registrar<T>::s_registered =
		Factory<Base, Args...>::Registrar<T>::RegisterType();
}

/*
Usage of the Factory:
class Base : public Nui::Factory<Base, Args...>
{
public:
	Base(Key) {}
	// ...
};

class Derived : public Base::Registrar<Derived>
{
public:
	// The constructor arguments have to be the same as defined in the base inheritance template
	Derived(Args...) {}
};

// Usage:
std::unique_ptr<Derived> derivedPtr = Base::Create<Derived>(Args...);
*/