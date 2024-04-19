#pragma once
#include "Graphics/Common.h"

namespace Nui::Graphics::States
{
    /**
     * @brief Get the rasterizer state description for disabling culling.
     * @return A D3D11_RASTERIZER_DESC structure for rasterizer state with no culling.
     */
    static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateNoCullDesc();

    /**
     * @brief Get the rasterizer state description for front face culling.
     * @return A D3D11_RASTERIZER_DESC structure for rasterizer state with front face culling.
     */
    static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateFrontFaceCullDesc();

    /**
     * @brief Get the rasterizer state description for front face culling with scissor test.
     * @return A D3D11_RASTERIZER_DESC structure for rasterizer state with front face culling and scissor test.
     */
    static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateFrontFaceCullScissorDesc();

    /**
     * @brief Get the rasterizer state description for back face culling.
     * @return A D3D11_RASTERIZER_DESC structure for rasterizer state with back face culling.
     */
    static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateBackFaceCullDesc();

    /**
     * @brief Get the rasterizer state description for back face culling with scissor test.
     * @return A D3D11_RASTERIZER_DESC structure for rasterizer state with back face culling and scissor test.
     */
    static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateBackFaceCullScissorDesc();

    /**
     * @brief Get the rasterizer state description for disabling culling and multisampling.
     * @return A D3D11_RASTERIZER_DESC structure for rasterizer state with no culling and no multisampling.
     */
    static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateNoCullNoMSDesc();

    /**
     * @brief Get the rasterizer state description for disabling culling with scissor test.
     * @return A D3D11_RASTERIZER_DESC structure for rasterizer state with no culling and scissor test enabled.
     */
    static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateNoCullScissorDesc();

    /**
     * @brief Get the rasterizer state description for wireframe rendering.
     * @return A D3D11_RASTERIZER_DESC structure for rasterizer state with wireframe rendering enabled.
     */
    static constexpr D3D11_RASTERIZER_DESC GetRasterizerStateWireframeDesc();
}