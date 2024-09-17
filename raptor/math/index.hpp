#pragma once

#include "aabb.hpp"
#include "vector.hpp"

namespace rptr::math
{
    size_t linearize_index(const math::Vector3i &p_size, const math::Vector3i &p_index);
    int linearize_index(const AABBi &bounds, const Vector3i &index);

    Vector3i vectorize_index(const math::Vector3i &p_size, size_t p_index);
    Vector3i vectorize_index(const AABBi &bounds, int index);

} //namespace rptr::math