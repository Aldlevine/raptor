#pragma once

#include "grid_chunk.hpp"
#include "grid_defs.hpp"
#include "grid_helpers.hpp"

#include "math/math.hpp"

#include <unordered_map>

namespace rptr::grid
{
    template <typename Type_>
    class Grid
    {
    public:
        using Type = Type_;
        using ChunkMap = std::unordered_map<Index, GridChunk<Type>, Index::Hash>;

        const Type &getv(const Index &p_global_index) const
        {
            const auto chunk_index = get_chunk_index_for_global_index(p_global_index);
            if (!m_chunk_map.contains(chunk_index))
            {
                return m_clear_value;
            }

            const auto linear_index = math::linearize_index(math::AABBi{ chunk_index, CHUNK_SIZE }, p_global_index);
            return m_chunk_map.at(chunk_index).data()[linear_index];
        }

        void setv(const Index &p_global_index, const Type &p_value)
        {
            const auto chunk_index = get_chunk_index_for_global_index(p_global_index);
            const auto linear_index = math::linearize_index(math::AABBi{ chunk_index, CHUNK_SIZE }, p_global_index);
            m_chunk_map[chunk_index].data()[linear_index] = p_value;
        }

        void fill(const Bounds &p_global_bounds, const Type &p_value)
        {
            std::vector<Index> chunk_indices = get_chunk_indices_for_bounds(p_global_bounds);

            for (const auto &it : chunk_indices)
            {
                m_chunk_map.try_emplace(it);
            }

            // relies on chunks not sharing cache space
            // default allocator (in unordered_map) works well
#pragma omp parallel for schedule(dynamic)
            for (const auto &chunk_index : chunk_indices)
            {
                const Index &chunk_offset = chunk_index * CHUNK_SIZE;
                const Bounds chunk_bounds(chunk_offset, CHUNK_SIZE);

                if (p_global_bounds.encloses(chunk_bounds))
                {
                    auto &chunk = m_chunk_map.at(chunk_index).data();
                    std::fill(chunk.begin(), chunk.end(), p_value);
                    continue;
                }

                const Bounds &isect = p_global_bounds.intersection(chunk_bounds);
                const Index &begin = isect.position - chunk_offset;
                const Index &end = isect.get_end() - chunk_offset;

                auto &chunk = m_chunk_map.at(chunk_index);
                Index vindex{};

                for (int x = begin.x; x < end.x; x++)
                {
                    for (int y = begin.y; y < end.y; y++)
                    {
                        for (int z = begin.z; z < end.z; z++)
                        {
                            chunk.data()[math::linearize_index(CHUNK_SIZE, math::Vector3i(x, y, z))] = p_value;
                        }
                    }
                }
            }
        }

    private:
        ChunkMap m_chunk_map{};
        Type m_clear_value{};
    };

} // namespace rptr::grid
