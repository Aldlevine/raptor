#pragma once

#include "math/math.hpp"

namespace rptr::grid
{
    using Index = math::Vector3i;
    using Bounds = math::AABBi;

    constexpr Index CHUNK_SIZE{ 16, 16, 16 };

} //namespace rptr::grid
