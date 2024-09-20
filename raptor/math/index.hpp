#pragma once

#include "aabb.hpp"
#include "vector.hpp"

namespace rptr::math
{
    constexpr size_t linearize_index(const Vector3i &p_size, const Vector3i &p_index)
    {
        return ((p_index.x) + (p_index.y * p_size.x) +
                (p_index.z * p_size.x * p_size.y));
    }

    constexpr size_t linearize_index(const AABBi &p_bounds, const Vector3i &p_index)
    {
        return linearize_index(p_bounds.size, p_index - p_bounds.position);
    }

    constexpr Vector3i vectorize_index(const Vector3i &p_size, size_t p_index)
    {
        Vector3i offset;
        offset.x = static_cast<int32_t>(p_index) % p_size.x;
        offset.y = static_cast<int32_t>(p_index / p_size.x) % p_size.y;
        offset.z = static_cast<int32_t>(p_index) / (p_size.x * p_size.y);
        return offset;
    }

    constexpr Vector3i vectorize_index(const AABBi &p_bounds, int p_index)
    {
        return p_bounds.position + vectorize_index(p_bounds.size, p_index);
    }
} //namespace rptr::math