#pragma once
#include "Graphics/Common.h"

namespace Nui::Graphics::States
{
    /**
     * @brief Get the depth stencil description for disabling depth testing.
     * @return A D3D11_DEPTH_STENCIL_DESC structure for depth stencil with depth testing disabled.
     */
    static constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilDepthDisabledDesc();

    /**
     * @brief Get the depth stencil description for enabling depth testing.
     * @return A D3D11_DEPTH_STENCIL_DESC structure for depth stencil with depth testing enabled.
     */
    static constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilDepthEnabledDesc();

    /**
     * @brief Get the depth stencil description for enabling depth testing with reversed depth comparison.
     * @return A D3D11_DEPTH_STENCIL_DESC structure for depth stencil with reversed depth comparison enabled.
     */
    static constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilReverseDepthEnabledDesc();

    /**
     * @brief Get the depth stencil description for enabling depth writing.
     * @return A D3D11_DEPTH_STENCIL_DESC structure for depth stencil with depth writing enabled.
     */
    static constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilDepthWriteEnabledDesc();

    /**
     * @brief Get the depth stencil description for enabling depth writing with reversed depth comparison.
     * @return A D3D11_DEPTH_STENCIL_DESC structure for depth stencil with reversed depth comparison and depth writing enabled.
     */
    static constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilReverseDepthWriteEnabledDesc();

    /**
     * @brief Get the depth stencil description for enabling both depth and stencil writing.
     * @return A D3D11_DEPTH_STENCIL_DESC structure for depth stencil with both depth and stencil writing enabled.
     */
    static constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilDepthStencilWriteEnabledDesc();

    /**
     * @brief Get the depth stencil description for enabling stencil testing.
     * @return A D3D11_DEPTH_STENCIL_DESC structure for depth stencil with stencil testing enabled.
     */
    static constexpr D3D11_DEPTH_STENCIL_DESC GetDepthStencilStencilEnabledDesc();
}
