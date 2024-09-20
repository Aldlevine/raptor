#pragma once

#include "grid_defs.hpp"

namespace rptr::grid
{
    constexpr Index get_chunk_index_for_global_index(const Index &p_global_index)
    {
        return floor_div(p_global_index, CHUNK_SIZE);
    }

    constexpr std::vector<Index> get_chunk_indices_for_bounds(const Bounds &p_bounds)
    {
        const auto begin = floor_div(p_bounds.position, CHUNK_SIZE);
        const auto end = ceil_div(p_bounds.get_end(), CHUNK_SIZE);
        std::vector<Index> result;
        result.reserve((end - begin).product());

        for (int z = begin.z; z < end.z; ++z)
        {
            for (int y = begin.y; y < end.y; ++y)
            {
                for (int x = begin.x; x < end.x; ++x)
                {
                    result.emplace_back(x, y, z);
                }
            }
        }
        return result;
    }
} // namespace rptr::grid
