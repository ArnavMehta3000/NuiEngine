#include "Core/Common/Types.h"

namespace Nui::StringConv
{
    /**
     * @brief Convert string to wide string
     * @param str Input narrow string
     * @return Wide string
     */
    static WString ToWide(const String& str) noexcept;

    /**
     * @brief Convert wide string to narrow string
     * @param wstr Input wide string
     * @return Narrow string
     */
    static String ToNarrow(const WString& wstr) noexcept;
}