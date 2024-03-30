#pragma once
#include "Core/Utils/Factory.h"

namespace Nui::ECS
{

	class World : public Nui::Factory<World>
	{
	public:
		World(Key) {}
		virtual StringView GetName() = 0;
	};

	class TestWorld : public World::Registrar<TestWorld>
	{
	public:
		TestWorld() { }

		~TestWorld() { }
		StringView GetName() override
		{
			return "World<T>";
		}
	};

	class TestWorld2 : public World::Registrar<TestWorld2>
	{
	public:
		TestWorld2() = default;
		TestWorld2(std::string_view name)
			: m_name(name)
		{}

		~TestWorld2()
		{}

		StringView GetName() override
		{
			return "World<T>";
		}

	private:
		std::string m_name = "grnirg";
	};
}

