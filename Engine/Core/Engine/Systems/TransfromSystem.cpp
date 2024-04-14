#include "TransfromSystem.h"
#include "Core/Engine/Components/Transform.h"

using namespace Nui::Components;

namespace Nui::Systems
{
	namespace  // Anonymous namespace
	{
		std::shared_ptr<Transform> GetTransform(const Node& node)
		{
			ECS::Entity* e = node.GetOwner();

			if (!e)
			{
				return nullptr;
			}

			ECS::ComponentVector<Transform> transforms = e->Query<Transform>();

			return transforms.First();
		}
	}  // Anonymous namespace


	Systems::TransformSystem::TransformSystem()
	{
		this->BindCallback(std::bind(&TransformSystem::operator(), this, std::placeholders::_1));
	}

	bool TransformSystem::operator()(Components::Node& node)
	{
		if (!node.HasParent())
		{
			return true;
		}

		const Node& parent = *node.GetParent();

		std::shared_ptr<Transform> parentTransform = GetTransform(parent);
		std::shared_ptr<Transform> nodeTransform = GetTransform(node);

		nodeTransform->SetParent(parentTransform->GetWorldMatrix());

		return true;  // True means continue traversing down the hierarchy
	}
}
