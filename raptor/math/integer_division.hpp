#pragma once

#include "vector.hpp"

namespace rptr::math
{
    template <std::integral Int>
    constexpr Int floor_div(Int p_lhs, Int p_rhs)
    {
        Int result = p_lhs / p_rhs;
        if (p_lhs % p_rhs < 0)
        {
            --result;
        }
        return result;
    }

    template <std::integral Int, size_t t_size>
    constexpr math::Vector<Int, t_size> floor_div(
        const math::Vector<Int, t_size> &p_lhs,
        const math::Vector<Int, t_size> &p_rhs)
    {
        return [&]<size_t... t_it>(std::index_sequence<t_it...>)
        {
            return math::Vector<Int, t_size>{ floor_div(p_lhs[t_it], p_rhs[t_it])... };
        }(std::make_index_sequence<t_size>());
    }

    template <std::integral Int>
    constexpr Int ceil_div(Int p_lhs, Int p_rhs)
    {
        Int result = p_lhs / p_rhs;
        if (p_lhs % p_rhs > 0)
        {
            ++result;
        }
        return result;
    }

    template <std::integral Int, size_t t_size>
    constexpr math::Vector<Int, t_size> ceil_div(
        const math::Vector<Int, t_size> &p_lhs,
        const math::Vector<Int, t_size> &p_rhs)
    {
        return [&]<size_t... t_it>(std::index_sequence<t_it...>)
        {
            return math::Vector<Int, t_size>{ ceil_div(p_lhs[t_it], p_rhs[t_it])... };
        }(std::make_index_sequence<t_size>());
    }
} // namespace rptr::math