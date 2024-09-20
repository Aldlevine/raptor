#pragma once

#include "grid_cell.hpp"
#include "grid_chunk.hpp"
#include "grid_defs.hpp"
#include "grid_helpers.hpp"

#include <algorithm>
#include <unordered_map>

namespace rptr::grid
{
    /// @brief An unbounded multi-channel 3D grid
    /// @tparam t_Unit: The unit type (value type of each channel)
    /// @tparam t_channels: The number of channels
    template <UnitType t_Unit, size_t t_channels>
    class Grid
    {
    public:
        using m_Unit = t_Unit;
        constexpr static size_t m_channels = t_channels;

        using m_Chunk = GridChunk<m_Unit, m_channels>;
        using m_ChunkMap = std::unordered_map<Index, m_Chunk, Index::Hash>;

        auto &get(const Index &p_global_index) const
        {
            const auto chunk_index = get_chunk_index_for_global_index(p_global_index);
            if (!m_chunk_map.contains(chunk_index))
            {
                return m_clear_value;
            }

            const auto linear_index = math::linearize_index(
                { chunk_index * CHUNK_SIZE, CHUNK_SIZE }, p_global_index);

            return m_chunk_map.at(chunk_index)[linear_index];
        }

        void set(const Index &p_global_index, const CopiesToGridCell<m_Unit, m_channels> auto &p_value)
        {
            const auto chunk_index = get_chunk_index_for_global_index(p_global_index);
            const auto linear_index = math::linearize_index(
                { chunk_index * CHUNK_SIZE, CHUNK_SIZE }, p_global_index);

            m_chunk_map.try_emplace(chunk_index);
            m_chunk_map.at(chunk_index)[linear_index] = p_value;
        }

        void fill(const Bounds &p_global_bounds, const CopiesToGridCell<m_Unit, m_channels> auto &p_value)
        {
            std::vector<Index> chunk_indices = get_chunk_indices_for_bounds(p_global_bounds);

#pragma omp parallel for default(shared) schedule(static)
            for (const auto &it : chunk_indices)
            {
                if (!m_chunk_map.contains(it))
                {
                    auto chunk_pair = std::pair{ it, m_Chunk{} };
#pragma omp critical
                    {
                        m_chunk_map.emplace(std::move(chunk_pair));
                    }
                }
            }

            // relies on chunks not sharing cache space
            // default allocator (in unordered_map) works well
#pragma omp parallel for default(shared) schedule(dynamic)
            for (const auto &chunk_index : chunk_indices)
            {
                const Index &chunk_offset = chunk_index * CHUNK_SIZE;
                const Bounds chunk_bounds(chunk_offset, CHUNK_SIZE);

                auto &chunk = m_chunk_map.at(chunk_index);

                if (p_global_bounds.encloses(chunk_bounds))
                {
                    std::fill(chunk.begin(), chunk.end(), p_value);
                    continue;
                }

                const Bounds &isect = p_global_bounds.intersection(chunk_bounds);
                const Index &begin = isect.position - chunk_offset;
                const Index &end = isect.get_end() - chunk_offset;

                for (int z = begin.z; z < end.z; z++)
                {
                    for (int y = begin.y; y < end.y; y++)
                    {
                        for (int x = begin.x; x < end.x; x++)
                        {
                            const auto &linear_index = math::linearize_index(CHUNK_SIZE, Index(x, y, z));
                            chunk[linear_index] = p_value;
                        }
                    }
                }
            }
        }

    private:
        m_ChunkMap m_chunk_map{};
        typename m_Chunk::m_Cell m_clear_value{};
    };

} // namespace rptr::grid
