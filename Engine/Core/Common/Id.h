#pragma once
#include "Core/Utils/Log.h"
#include <typeinfo>

namespace Nui::Id
{
	using Type = U32;
	
	namespace Internal
	{
		constexpr U32  GenerationBits = 8;
		constexpr U32  IndexBit       = sizeof(Type) * 8 - GenerationBits;
		constexpr Type IndexMask      = (Type(1) << IndexBit) - 1;
		constexpr Type GenerationMask = (Type(1) << GenerationBits) - 1;
	}

	constexpr Type InvalidId = Type(-1);
	constexpr U32 MinDeletedCount = 1024;

	using GenerationType = std::conditional_t<Internal::GenerationBits <= 16, std::conditional_t<Internal::GenerationBits <= 8, U8, U16>, U32>;

	static_assert(sizeof(GenerationType) * 8 >= Internal::GenerationBits, "\'GenerationType\' must be big enough to hold \'GenerationBits\'");
	static_assert((sizeof(Type) - sizeof(GenerationType)) > 0, "\'Type\' must be big enough to hold \'IndexBit\' + \'GenerationBits\'");

	inline constexpr bool IsValid(Type id)
	{
		return id != InvalidId;
	}

	inline Type Index(Type id)
	{
		Type index = id & Internal::IndexMask;
		NUI_ASSERT(index != Internal::IndexMask, "Index overflow");
		return id & Internal::IndexMask;
	}

	inline constexpr Type Generation(Type id)
	{
		return (id >> Internal::IndexBit) & Internal::GenerationMask;
	}

	inline Type NewGeneration(Type id)
	{
		const Type gen = Generation(id) + 1;
		NUI_ASSERT(gen < (((U64)1 << Internal::GenerationBits) - 1), "Generation overflow");
		return Index(id) | (gen << Internal::IndexBit);
	}

#ifdef NUI_DEBUG
	namespace Internal
	{
		struct IdBase
		{
			constexpr explicit IdBase(Type id) : Id(id) {}
			constexpr operator Type() const { return Id; }

		private:
			Type Id;
		};
	}
#define NUI_DEFINE_TYPED_ID(name)                      \
struct name final : Nui::Id::Internal::IdBase          \
{                                                      \
	constexpr explicit name(Nui::Id::Type id)          \
		: Nui::Id::Internal::IdBase(id) {}             \
	constexpr name() : Nui::Id::Internal::IdBase(0) {} \
};
#else
#define NUI_DEFINE_TYPED_ID(name) using name = Id::Type;
#endif
}