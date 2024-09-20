#pragma once

#include <tuple>

namespace rptr::utils
{
    /// @brief Tuple contains only the specified element type (defaults to the 1st)
    /// @tparam t_Tuple: The tuple type to check
    /// @tparam t_Element: The element type to check
    template <typename t_Tuple, typename t_Element = std::tuple_element_t<0, t_Tuple>>
    constexpr bool is_homogenous_tuple_of_type = []<std::size_t... I>(std::index_sequence<I...>)
    {
        return (std::same_as<std::tuple_element_t<I, t_Tuple>, t_Element> && ...);
    }(std::make_index_sequence<std::tuple_size_v<t_Tuple>>());

} // namespace rptr::utils