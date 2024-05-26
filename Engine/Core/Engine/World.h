#pragma once
#include "Core/Common/CommonHeaders.h"
#include "Core/Engine/ECS/ECS.h"
#include "Core/Engine/Components/TransformComponent.h"

namespace Nui
{
	// Forward declarations
	namespace Systems { class TransformSystem; }

	/**
	 * @brief World class - the main context for the engine
	 */
	class World final : public ECS::Context
	{
		friend class AppBase;

	public:
		/**
		 * @brief World constructor - initializes the ECS context
		 */
		World();

		/**
		 * @brief Default destructor
		 */
		~World() = default;
		
		World(const World&) = delete;
		World(World&&) = delete;

		World operator=(const World&) = delete;
		World operator=(World&&) = delete;

	private:
		/**
		 * @brief Called by AppBase to initialize the world
		 */
		void OnInit();

		/**
		 * @brief Called by AppBase every frame to update the world
		 * @param dt 
		 */
		void Update(const F64 dt);

		/**
		 * @brief Called by AppBase to shutdown the world
		 */
		void OnShutdown();

	private:
		/**
		 * @brief Pointer to the transform system in the ECS Context
		 */
		Systems::TransformSystem* m_transformSystem;
	};
}