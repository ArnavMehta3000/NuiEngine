#pragma once
#include "Graphics/Common.h"

namespace Nui::Graphics
{
	class BatchRenderer
	{
	public:
		enum FilterMode
		{
			None = 0,
			Linear = 1,
			Point = 2
		};

		static constexpr U32 MAX_BATCH_SIZE = 1024;

		struct SpriteDrawData
		{
			Math::Matrix Transform;
			Math::Vector4 Tint;
			Math::Vector4 DrawRect;
		};

		BatchRenderer(ID3D11Device1* dev);
		~BatchRenderer();

		void BeginFrame(ID3D11DeviceContext1* ctx, FilterMode filterMode = FilterMode::None);

		void Draw();
	};
}