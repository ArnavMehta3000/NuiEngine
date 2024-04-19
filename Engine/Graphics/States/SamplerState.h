#pragma once
#include "Graphics/Common.h"

namespace Nui::Graphics
{
    /**
     * @brief Get the sampler state description for linear filtering.
     * @return A D3D11_SAMPLER_DESC structure for sampler state with linear filtering.
     */
    static constexpr D3D11_SAMPLER_DESC GetSamplerStateLinearDesc();

    /**
     * @brief Get the sampler state description for linear filtering with clamping.
     * @return A D3D11_SAMPLER_DESC structure for sampler state with linear filtering and clamping.
     */
    static constexpr D3D11_SAMPLER_DESC GetSamplerStateLinearClampDesc();

    /**
     * @brief Get the sampler state description for linear filtering with border sampling.
     * @return A D3D11_SAMPLER_DESC structure for sampler state with linear filtering and border sampling.
     */
    static constexpr D3D11_SAMPLER_DESC GetSamplerStateLinearBorderDesc();

    /**
     * @brief Get the sampler state description for point filtering.
     * @return A D3D11_SAMPLER_DESC structure for sampler state with point filtering.
     */
    static constexpr D3D11_SAMPLER_DESC GetSamplerStatePointDesc();

    /**
     * @brief Get the sampler state description for anisotropic filtering.
     * @return A D3D11_SAMPLER_DESC structure for sampler state with anisotropic filtering.
     */
    static constexpr D3D11_SAMPLER_DESC GetSamplerStateAnisotropicDesc();
}
