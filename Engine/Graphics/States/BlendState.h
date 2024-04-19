#pragma once
#include "Graphics/Common.h"

namespace Nui::Graphics::States
{
    /**
     * @brief Get the blend state description for disabling blending.
     * @return A D3D11_BLEND_DESC structure for blend state with blending disabled.
     */
    static constexpr D3D11_BLEND_DESC GetBlendStateBlendDisabledDesc();

    /**
     * @brief Get the blend state description for additive blending.
     * @return A D3D11_BLEND_DESC structure for blend state with additive blending.
     */
    static constexpr D3D11_BLEND_DESC GetBlendStateAdditiveBlendDesc();

    /**
     * @brief Get the blend state description for alpha blending.
     * @return A D3D11_BLEND_DESC structure for blend state with alpha blending.
     */
    static constexpr D3D11_BLEND_DESC GetBlendStateAlphaBlendDesc();

    /**
     * @brief Get the blend state description for premultiplied alpha blending.
     * @return A D3D11_BLEND_DESC structure for blend state with premultiplied alpha blending.
     */
    static constexpr D3D11_BLEND_DESC GetBlendStatePreMultipliedAlphaBlendDesc();

    /**
     * @brief Get the blend state description for disabling color write.
     * @return A D3D11_BLEND_DESC structure for blend state with color write disabled.
     */
    static constexpr D3D11_BLEND_DESC GetBlendStateColorWriteDisabledDesc();

    /**
     * @brief Get the blend state description for alpha to coverage blending.
     * @return A D3D11_BLEND_DESC structure for blend state with alpha to coverage blending.
     */
    static constexpr D3D11_BLEND_DESC GetBlendStateAlphaToCoverageDesc();

    /**
     * @brief Get the blend state description for opacity blending.
     * @return A D3D11_BLEND_DESC structure for blend state with opacity blending.
     */
    static constexpr D3D11_BLEND_DESC GetBlendStateOpacityBlendDesc();
}
